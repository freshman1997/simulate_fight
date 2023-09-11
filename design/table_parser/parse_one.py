import xlwings as xw
import json
from enum import Enum

class ValueType(Enum):
    int = 1,
    float = 2,
    string = 3,
    array = 4,
    map = 5

def get_pure_name(str):
    for i in range(len(str)):
        if str[i] == '(':
            return str[:i]
        
    return None
            
def parse_type(str):
    l = len(str)
    for i in range(l):
        if str[i] == '(':
            for j in range(i + 1, l):
                if str[j] == ')':
                    return str[i + 1 : j]
    return None

def translate_type(str):
    if str == 'int':
        return ValueType.int
    elif str == 'float':
        return ValueType.float
    elif str == 'string':
        return ValueType.string
    elif str == 'array':
        return ValueType.array
    elif str == 'map':
        return ValueType.map
    else:
        return None

def parse_one(prefix, file, sheet, output):
    app = xw.App(visible=True,add_book=False)
    wb = app.books.open(prefix + "/" + file)
    sh = wb.sheets[sheet]

    header = {}
    # read meta data
    for r in range(1, 7):
        for _ in range(1, 2):
            header[sh["A" + str(r)].value] = [sh["B" + str(r)].value, sh["C" + str(r)].value]

    if len(header['key_row']) <= 0:
        return False
    
    class meta:
        def __init__(self) -> None:
            self.start_row = 0
            self.end_row = -1
            self.meta_data = {}
        

    metas = [0] * 2
    f = int(ord('A'))
    metas[0] = meta()
    metas[1] = meta()

    r1 = header['key_row'][0]
    r2 = header['key_row'][1]
    count = 0
    metas[0].start_row = int(header['start_row'][0]);
    metas[1].start_row = int(header['start_row'][1]);

    v1 = sh[chr(f) + str(int(r1))].value
    v2 = sh[chr(f) + str(int(r2))].value
    metas[0].meta_data['key'] = []
    metas[0].meta_data['type'] = []
    metas[1].meta_data['key'] = []
    metas[1].meta_data['type'] = []
    
    while True:
        v1 = sh[chr(f + count) + str(int(r1))].value
        v2 = sh[chr(f + count) + str(int(r2))].value
        if v1 == None and v2 == None:
            break
        
        key1 = get_pure_name(v1)
        key2 = get_pure_name(v2)
        type1 = translate_type(parse_type(v1))
        type2 = translate_type(parse_type(v2))
        
        metas[0].meta_data['key'].append(key1)
        metas[1].meta_data['key'].append(key2)
        
        metas[0].meta_data['type'].append(type1)
        metas[1].meta_data['type'].append(type2)
        count = count + 1
        
    metas[0].end_row = count
    metas[1].end_row = count
    #for i in 0, 1:
    #    print("{ start_row = %d, end_row = %d, metas = %s" % (metas[i].start_row, metas[i].end_row, metas[i].meta_data))

    datas = []
    ref = metas[0]
    j = metas[0].start_row
    while(True):
        count = metas[0].end_row
        key = sh[chr(f) + str(j)].value
        if key == None:
            break
        
        i = 0
        tmp = {}
        while i < count:
            if sh[chr(f + i) + str(j)].value == None:
                i = i + 1
                continue
            
            # 读取一行
            #print("name: %s, type: %s, i: %d" % (ref.meta_data['key'][i], ref.meta_data['type'][i], i))
            if ref.meta_data['type'][i] == ValueType.int:
                tmp[ref.meta_data['key'][i]] = int(sh[chr(f + i) + str(j)].value)
            elif ref.meta_data['type'][i] == ValueType.float:
                tmp[ref.meta_data['key'][i]] = float(sh[chr(f + i) + str(j)].value)
            elif ref.meta_data['type'][i] == ValueType.string:
                tmp[ref.meta_data['key'][i]] = sh[chr(f + i) + str(j)].value
            elif ref.meta_data['type'][i] == ValueType.array:
                tmp[ref.meta_data['key'][i]] = eval(sh[chr(f + i) + str(j)].value)
            elif ref.meta_data['type'][i] == ValueType.map:
                tmp[ref.meta_data['key'][i]] = eval(sh[chr(f + i) + str(j)].value)
            
            i = i + 1
        
        datas.append(tmp)
        j = j + 1
        
    json_doc = json.dumps(datas, sort_keys=True, indent=4, separators=(',', ':'))
    json_doc = json_doc.replace('u\'','\'').encode().decode("unicode-escape")
    f = open(output, mode = "w+", encoding="utf-8")
    f.write(json_doc)
    f.close()

    app.kill()
    
    return True
