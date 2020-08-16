strliteral_gen=generator(strliteral,output:'@PLAINNAME@.hpp',arguments:['-i','@INPUT@','-o','@OUTPUT@','@EXTRA_ARGS@'])
fs=import('fs')

assert(fs.is_file('empty.test'),'empty.test file doesn\'t exist')
assert(fs.size('empty.test')==0,'empty.test file isn\'t empty')
empty=strliteral_gen.process('empty.test',extra_args:'empty_test')

assert(fs.is_file('random.test'),'random.test file doesn\'t exist')
random=strliteral_gen.process('random.test',extra_args:'random_test')

assert(fs.is_file('big_random.test'),'big_random.test file doesn\'t exist')
big_random=strliteral_gen.process('big_random.test',extra_args:'big_random_test')

catch2=subproject('catch2').get_variable('catch2_dep')

strliteral_test=executable('strliteral_test',['strliteral_test.cpp',empty,random,big_random],dependencies:catch2)

test('string literal test',strliteral_test,args:['-s'])
