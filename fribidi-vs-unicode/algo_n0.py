#!/usr/bin/python

import bd16
import pandas as pd

# Identify the bracket pairs in the current isolating run seq
#pairings = bd16.find_pairings(text)

def is_strong(bc):
  return bc in ['R','L']

def get_bidi_class(ch):
  if ch in 'ABCDEFGHIJKLMNOPQRSTUVWXYZ':
    return 'R'
  if ch in 'abcdefghijklmnopqrstuvwxyz':
    return 'L'
  if ch in '({[]})&!':
    return 'ON'
  if ch in ' ':
    return 'WS'

def find_strong(bidi_classes, start, end):
  '''find a strong character between start and end non-inclusive'''
  rdir = 1
  if end < start:
    if end == -1:
      end = None   # Req of the string range operator
    rdir = -1

  for bc in bidi_classes[start:end:rdir]:
    if is_strong(bc):
      return bc

  return None

def find_matching_strong(bidi_classes, start, end, match):
  '''Used to find a character between start and end with the
  bidiclass match'''
  for i in range(start,end):
    if bidi_classes[i] == match:
      return True

  return False

def test_n0(text,embedding_direction, print_pairings=False):
  Table = [list(text) + ['']]
  bc = [get_bidi_class(ch) for ch in text]
  Table += [bc[:] + ['']]
  
  pairings = bd16.find_pairings(text)
  if print_pairings:
    print pairings
  for pr in pairings:
    rule = ''
    other_direction = {'L':'R','R':'L'}[embedding_direction]
    has_matching_strong = find_matching_strong(bc, pr[0], pr[1], embedding_direction)
    has_other_strong = find_matching_strong(bc, pr[0], pr[1], 
                                            other_direction)
    preceding_strong = find_strong(bc, pr[0], -1)
    print '(%d,%d) has_matching_strong has_other_strong preceding_strong='%(pr[0],pr[1]),has_matching_strong,has_other_strong, preceding_strong
  
    # Rule N0b
    if has_matching_strong:
      rule= 'N0b: %s->%s'%(bc[pr[0]],embedding_direction)
      bc[pr[0]]=bc[pr[1]]=embedding_direction
  
    elif has_other_strong:
      # Rule N0c1
      if preceding_strong == other_direction:
        rule= 'N0c1: %s->%s'%(bc[pr[0]],other_direction)
        bc[pr[0]]=bc[pr[1]]=other_direction
      else:
        rule= 'N0c2: %s->%s'%(bc[pr[0]],embedding_direction)
        bc[pr[0]]=bc[pr[1]]=embedding_direction
    else:
      rule= 'N0d'
      # N0d - Do nothing
  
    Table += [bc[:]+[rule,pr]]
  df =  pd.DataFrame(Table,
                     columns=range(len(Table[0])-1)+['Rule','Pair'])
  df.Pair.fillna(value='',inplace=True)
  print df
  print ''
  

#test_n0(text='AB(CD[&ef]!)gh',embedding_direction='R', print_pairings=True)
#test_n0(text='sm (fa AR) HE',embedding_direction='R')
#test_n0(text='AR bo(s)',embedding_direction='R')
#test_n0(text='(a)(b)c',embedding_direction='R', print_pairings=True)
test_n0(text='a(b)).A',embedding_direction='R', print_pairings=True)
