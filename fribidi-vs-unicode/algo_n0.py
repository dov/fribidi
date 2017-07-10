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
  if end < start:
    for i in range(start,end,-1):
      if is_strong(bidi_classes[i]):
        return bidi_classes[i]

  for bc in bidi_classes[start:end]:
    if is_strong(bc):
      return bc

  return None

def find_matching_strong(bidi_classes, start, end, match):
  for i in range(start,end):
    if bidi_classes[i] == match:
      return True

  return False

def test_n0(text,embedding_direction):
  Table = [list(text) + ['']]
  bc = [get_bidi_class(ch) for ch in text]
  Table += [bc[:] + ['']]
  
  pairings = bd16.find_pairings(text)
  for pr in pairings:
    rule = ''
    has_strong = find_matching_strong(bc, pr[0], pr[1], embedding_direction)
    preceding_strong = find_strong(bc, pr[0], -1)
  #  print '(%d,%d) strong preceding_strong='%(pr[0],pr[1]),strong,preceding_strong
  
    # Rule N0b
    if has_strong:
      rule= 'N0b: %s->%s'%(bc[pr[0]],embedding_direction)
      bc[pr[0]]=bc[pr[1]]=embedding_direction
  
    elif preceding_strong is not None:
      # Rule N0c1
  #    print 'preceding_strong embedding_direction=',preceding_strong,embedding_direction
      if preceding_strong != embedding_direction:
        rule= 'N0c1: %s->%s'%(bc[pr[0]],preceding_strong)
        bc[pr[0]]=bc[pr[1]]=preceding_strong
      # Rule N0c2
      else:
        rule= 'N0c2: %s->%s'%(bc[pr[0]],embedding_direction)
        bc[pr[0]]=bc[pr[1]]=embedding_direction
    else:
      rule= 'N0d'
      # N0d - Do nothing
  
    Table += [bc[:]+[rule]]
  print pd.DataFrame(Table,
                     columns=range(len(Table[0])-1)+['Rule']).to_string()
  print ''
  

test_n0(text='AB(CD[&ef]!)gh',embedding_direction='R')

test_n0(text='sm (fa AR) HE',embedding_direction='R')
  
test_n0(text='AR bo(s)',embedding_direction='R')
