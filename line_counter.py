import os

h_cnt = src_cnt = 0
h_lines = src_lines = 0

def add(filename):
    f = open(filename, 'r')
    return len(f.readlines())

for root, dirs, files in os.walk(".", topdown = False):
   for name in files:
      s = os.path.join(root, name)
      if s.find('cmake-build-debug') != -1: continue
      if s[-2:] == '.h':
          h_lines += add(s)
          h_cnt += 1
      elif s[-4:] == '.cpp':
          src_lines += add(s)
          src_cnt += 1

print('%d headers, sum of %d lines' %(h_cnt, h_lines))
print('%d src files, sum of %d lines' %(src_cnt, src_lines))
