#!/usr/bin/python
# -*- Encoding: utf-8 -*-

# A viewer for the BidiCharacterTest.txt file.
#
# Dov Grobgeld
# 2017-01-03 Tue

import sys
import gtk
import cairo
import pango
import pangocairo as pc

def button_image(stock):
  image = gtk.Image()
  image.set_from_stock(stock,48)
  button = gtk.Button()
  button.set_image(image)
  button.set_label("")
  return button

class ResView(gtk.DrawingArea):
  __gsignals__ = { "expose-event": "override" }

  def do_expose_event(self,event):
    cr = self.window.cairo_create()

    self.draw(cr, *self.window.get_size())

  def show_text(self, cr, x,y,text, font = 'monospace 18', align=0, color=None):
    pc_ctx = pc.CairoContext(cr)
    pc_layout = pc_ctx.create_layout()
    desc = pango.FontDescription(font)
    pc_layout.set_font_description(desc)
    if color is None:
      color = (0,0,0)
    pc_ctx.set_source_rgb(*color)
    pc_layout.set_text(text)
    pc_ctx.update_layout(pc_layout)
    width, height = pc_layout.get_size()
    x -= align * width/pango.SCALE
    cr.move_to(x,y)
    pc_ctx.show_layout(pc_layout)
    x+= 1.0*width/pango.SCALE
    return (x,y)

  def show_char(self,cr,x,y,c,font='monospace 18'):
    '''Show characters including display of special characters'''
    c = unicode(c)
    
    special = {
      unichr(0x202a) : 'LRE',
      unichr(0x202b) : 'RLE',
      unichr(0x202c) : 'PDF',
      unichr(0x202d) : 'LRO',
      unichr(0x202e) : 'RLO',
      unichr(0x2066) : 'LRI',
      unichr(0x2067) : 'RLI',
      unichr(0x2068) : 'FSI',
      unichr(0x2069) : 'PDI',
    }
    visual = {' ':u'⎵'}

    if c in special:
      self.show_text(cr,x,y+8,special[c], font='monospace 8',align=0.5, color=(0,0,1))
    else:
      c = visual.get(c,c)
      self.show_text(cr,x,y,c,font=font, align=0.5)
      
  def draw(self, cr, width, height):
    # Fill the background with gray
    cr.set_source_rgb(0.8, 0.8, 0.75)
    cr.rectangle(0, 0, width, height)
    
    cr.fill()

    dx = 30   # char spacing
    dy = 40   # line spacing
    x1 = 200  # x margin for second column
    x,y = x1,100
    self.show_text(cr, 10,y, 'Dir:')
    self.show_text(cr,x,y,self.para_dir)

    x,y = x1,y+dy
    self.show_text(cr, 10,y, 'Input:')
    for c in self.unichars:
      self.show_char(cr,x,y,c)
      x+= dx

    x,y = x1,y+dy
    self.show_text(cr, 10,y, 'Expected:')
    for pos in self.order:
      c = self.unichars[pos]
      self.show_char(cr,x,y,c)
      x+= dx

    x,y = x1,y+dy
    self.show_text(cr, 10,y, 'Level:')
    for v in self.level:
      self.show_text(cr,x,y,str(v),font='monospace 12', align=0.5)
      x+= dx

    x,y = x1,y+dy
    self.show_text(cr, 10,y, 'Order:')
    for pos in self.order:
      self.show_text(cr,x,y,str(pos),font='monospace 12',align=0.5)
      x+= dx

  def set_res(self, TestString):
    fields = TestString.split(';')
    self.unichars = ''.join([unichr(int(v,16)) for v in fields[0].split(' ')])
    self.unichars.replace(' ','_')
    self.para_dir = {0: 'LTR', 1:'RTL',2:'auto'}[int(fields[1])]
    self.order = [int(v) for v in fields[4].split()]
    self.level = [v for v in fields[3].split()]
    self.queue_draw()

class App(gtk.Window):
  def __init__(self, filename):
    fh = open(filename)
    lines = []
    self.line_num_to_line_idx = {}  # A map for the go to line button
    for linenum,line in enumerate(fh):
      if line[0]=='#' or len(line)<2:
        continue
      lines+=[(linenum,line)]
      self.line_num_to_line_idx[linenum+1] = len(lines)-1

    self.filename = filename
    self.test_list = lines
    self.test_index = 0
    gtk.Window.__init__(self,gtk.WINDOW_TOPLEVEL)
    vbox = gtk.VBox(0,False)
    self.add(vbox)
    vbox.show()
    hbox = gtk.HBox(0,False)
    bbox = gtk.HButtonBox()
    hbox.pack_start(bbox, 0,0,10)
    self.info_label = gtk.Label('Info label')
    self.info_label.set_alignment(0,0.5)
    hbox.pack_start(self.info_label, 1,1,0)
    self.button_back = button_image(gtk.STOCK_GO_BACK)
    self.button_fwd = button_image(gtk.STOCK_GO_FORWARD)
    self.button_back.connect('clicked',self.on_back)
    self.button_fwd.connect('clicked',self.on_fwd)
    bbox.add(self.button_back)
    bbox.add(self.button_fwd)
    self.entry = gtk.Entry()
    self.entry.connect('activate', self.on_entry_activate)
    hbox.pack_start(gtk.Label('Line:'), 0,0,0)
    hbox.pack_start(self.entry, 0,0,0)
    self.button_go = button_image(gtk.STOCK_OK)
    self.button_go.connect('clicked',self.on_go)
    hbox.pack_start(self.button_go, 0,0,0)
    vbox.pack_start(hbox, 1,1,0)
    self.uniview = ResView()
#    self.uniview.set_res('05D0 05D1 0028 05D2 05D3 005B 0026 0065 0066 005D 002E 0029 0067 0068;0;0;1 1 0 1 1 0 0 0 0 0 0 0 0 0;1 0 2 4 3 5 6 7 8 9 10 11 12 13')
    self.update_display()

    self.uniview.set_size_request(800,600)
    vbox.pack_start(self.uniview, 1,1,0)

    vbox.show_all();
    self.connect("delete_event", self.delete_event)
    self.show()

  def delete_event(self,widget,event,data=None):
    gtk.mainquit()

  def update_display(self):
    lineno,line = self.test_list[self.test_index]
    self.info_label.set_text('%s: Showing test %d'%(self.filename,
                                                    self.test_index))
    self.entry.set_text(str(lineno+1))
    self.uniview.set_res(line)

  def on_fwd(self, event):
    self.test_index = (self.test_index + 1)%len(self.test_list)
    self.update_display()

  def on_go(self, event):
    self.test_index = self.line_num_to_line_idx[int(self.entry.get_text())]
    self.update_display()

  def on_entry_activate(self, event):
    self.test_index = self.line_num_to_line_idx[int(self.entry.get_text())]
    self.update_display()

  def on_back(self, event):
    self.test_index -= 1
    if self.test_index < 0:
      self.test_index = len(self.test_list)-1
    self.update_display()

if __name__ == "__main__":
  if len(sys.argv) < 2:
    filename = 'BidiCharacterTest.txt'
  else:
    filename = sys.argv[1]
  app = App(filename)
  settings = gtk.settings_get_default()
  settings.props.gtk_button_images = True
  gtk.main()

