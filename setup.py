
import os

prefix = '/// '
include_dir = 'include/'
doc_dir = 'doc/src'

def write_to(dir_name, file, lines):
    content = list((line + "\n" for line in lines))
    if (len(content) > 0):
        print('Writing file:', file)
        open(os.path.join(dir_name, file), 'w').writelines(content)

def insert_header(lines, file):
    for line in lines:
        yield line
        if line.startswith('=='):
            yield ""
            yield "Header"
            yield "------"
            yield ""
            yield "    #include <{0}>".format(file)
            yield ""

def write_header(md, name, file):
    include_file = file[len(include_dir):]
    write_to(doc_dir, name + '.md', insert_header(md, include_file))

def extract_md(file):
    f = open(file)
    return [line[len(prefix):-1] for line in f.readlines() if line.startswith(prefix)]

for root, subdirs, files in os.walk(include_dir):
    print("root", root)
    if 'detail' not in root:
        for f in files: 
            file = os.path.join(root, f)
            md = extract_md(file)
            name, ext = os.path.splitext(os.path.basename(file))
            write_header(md, name, file)

