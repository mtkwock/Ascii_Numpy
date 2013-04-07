array = eval(open('char_info.txt', 'r').read().split(' = ')[1])
array = [[array[j][i]/128 for i in range(162) if (i > 8 and i < 153 and i % 9 != 8)] for j in range(len(array))]
array = str([[hex(sum([array[j][i*8:(i+1)*8][k]*2**(8-k) for k in range(8)])) for i in range(16)] for j in range(len(array))]).replace('[', '{').replace(']', '}').replace("'", "")
print "const unsigned char[][] = " + array
