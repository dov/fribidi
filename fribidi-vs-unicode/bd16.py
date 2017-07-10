######################################################################
# An implementation of bd16 in python
#
# Dov Grobgeld <dov.grobgeld@gmail.com>
# 2017-01-10 Tue
######################################################################

from collections import namedtuple

Element = namedtuple('Element',['char','pos'])
BidiProp = namedtuple('BidiProp',['type','opening'])

brack_prop = {
  '(': BidiProp(1,True),   ')' :BidiProp(1,False),
  '[': BidiProp(2,True),   ']' :BidiProp(2,False),
  '{': BidiProp(3,True),   '}' :BidiProp(3,False),
}
  
def find_pairings(text):
  stack = []  # Stack of elements
  pairs = []

  for p,c in enumerate(text):
    if c in brack_prop:
      prop = brack_prop[c]
      if prop.opening:
        stack += [Element(c,p)]
      else:
        sp = len(stack)-1
        for i in range(sp,-1,-1):
          se = stack[i]
          se_prop = brack_prop[se.char]
          if se_prop.type == prop.type:
            stack = stack[:i]
            pairs += [(se.pos, p)]
            break
  return tuple(sorted(pairs, key=lambda v: v[0]))

if __name__ == '__main__':
  Test = namedtuple('Test',['text','exp_ranges'])
  
  tests = [
    Test('a)b(c', tuple()),
    Test('a(b]c', tuple()),
    Test('a(b)c', ((1,3),)),
    Test('a(b[c)d]', ((1,5),)),
    Test('a(b]c)d', ((1,5),)),
    Test('a(b)c)d', ((1,3),)),
    Test('a(b(c)d', ((3,5),)),
    Test('a(b(c)d)',((1,7),(3,5))),
    Test('a(b{c}d)',((1,7),(3,5)))
  ]
  
  for t in tests:
    res = find_pairings(t.text)
    print res, t.exp_ranges == res
  
