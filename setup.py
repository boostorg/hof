
import os

prefix = '/// '
include_dir = 'fit/'
doc_dir = 'doc/src'

def extract_md(file):
    f = open(file)
    return [line[len(prefix):] for line in f.readlines() if line.startswith(prefix)]



files = (path for f in os.listdir(include_dir) for path in [os.path.join(include_dir,f)] if os.path.isfile(path))
for file in files:
    md = extract_md(file)
    if len(md) > 0:
        name, ext = os.path.splitext(os.path.basename(file))
        open(os.path.join(doc_dir, name + '.md'), 'w').writelines(extract_md(file))
