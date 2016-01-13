
import os

prefix = '/// '
include_dir = 'fit/'
doc_dir = 'doc/src'

def insert_header_each(line, file):
    if line.startswith('=='):
        return [line, "\n", "Header\n", "------\n", "\n", "    #include <{0}{1}.h>\n".format(include_dir, file), "\n"]
    else:
        return [line]

def insert_header(lines, file):
    return [line for x in lines for line in insert_header_each(x, file)]

def extract_md(file):
    f = open(file)
    return [line[len(prefix):] for line in f.readlines() if line.startswith(prefix)]


files = (path for f in os.listdir(include_dir) for path in [os.path.join(include_dir,f)] if os.path.isfile(path))
for file in files:
    md = extract_md(file)
    if len(md) > 0:
        print('Extracting:', file)
        name, ext = os.path.splitext(os.path.basename(file))
        open(os.path.join(doc_dir, name + '.md'), 'w').writelines(insert_header(md, name))
