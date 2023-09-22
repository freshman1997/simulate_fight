import parse_one as parser
import config as cfg
import sys
import os

def parse_one_sheet(v):
    for i in range(len(v)):
        tb = v[i]
        res = parser.parse_one("design", tb["parse_file"], tb["parse_sheet"], "design/jsons/" + tb["output"])
        if not res:
            print("parse table %s:%s fail!" % tb["parse_file"], tb["parse_sheet"]) 
        else:
            print("[success] %s:%s ===> %s" % (tb["parse_file"], tb["parse_sheet"], "design/jsons/" + tb["output"]))

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("fail!")
        exit(-1)
    
    if sys.argv[1] == 'all':
        for k in cfg.xls_config:
            v = cfg.xls_config[k]
            parse_one_sheet(v)
    elif sys.argv[1] in cfg.xls_config:
        v = cfg.xls_config[sys.argv[1]]
        parse_one_sheet(v)
    else:
        print("NOT FOUND: %s" % sys.argv[1])
