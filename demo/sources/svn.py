s = open('../../.svn/entries', 'r')
s.readline()
s.readline()
s.readline()
svn_revision = s.readline()
s.readline()
s.readline()
s.readline()
s.readline()
s.readline()
svn_date = s.readline()
s.close()

f = open('svn.inc', 'w')
f.write('static const unsigned int SVN_REVISION = ' + svn_revision.strip() + ';\n')
f.write('static const char SVN_DATE[] = "' + svn_date.strip() + '";\n')
f.close()