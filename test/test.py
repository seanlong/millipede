import dbus
import os
import unittest
from bs4 import BeautifulSoup

manager_interface = 'org.seanlong.ClawerDriver.Manager'

def connect_manager():
  bus_name = 'org.seanlong.ClawerDriver'
  manager_path = '/Manager'
  bus = dbus.SessionBus()
  return bus.get_object(bus_name, manager_path)

class TestClawerManager(unittest.TestCase):
  error_name = 'org.seanlong.ClawerDriver.Manager.Error'

  def setUp(self):
    self._manager_obj = connect_manager()

  def test_echo(self):
    ret = self._manager_obj.Echo('123', dbus_interface=manager_interface)
    self.assertEqual(ret, '123')

  def test_set_pool_size(self):
    self._manager_obj.SetPoolSize(10, dbus_interface=manager_interface)

  def test_get_html_http(self):
    return
    self._manager_obj.SetPoolSize(-1, dbus_interface=manager_interface)
    ret = self._manager_obj.GetHTML('http://www.baidu.com',
                                    dbus_interface=manager_interface)
    self.assertTrue(len(ret) > 0)

  def test_get_html_url_ex(self):
    self._manager_obj.SetPoolSize(-1, dbus_interface=manager_interface)
    with self.assertRaises(dbus.exceptions.DBusException) as ex:
      self._manager_obj.GetHTML('bad.url', dbus_interface=manager_interface)
    self.assertEqual(ex.exception.message, 'Invalid URL')

  def test_get_html_timeout_ex(self):
    self._manager_obj.SetPoolSize(-1, dbus_interface=manager_interface)
    with self.assertRaises(dbus.exceptions.DBusException) as ex:
      self._manager_obj.GetHTML('http://www.unknown-will-timeout.com',
                                ';', #trigger timeout
                                dbus_interface=manager_interface,
                                timeout = 100) #should > 60 second
    self.assertTrue(ex.exception.message.find('timeout') > 0)
      

  def test_get_html_file(self):
    return
    self._manager_obj.SetPoolSize(-1, dbus_interface=manager_interface)
    hello_file_path = os.getcwd() + '/data/hello.html'  
    hello_file = open(hello_file_path, 'r')
    hello_html = self._manager_obj.GetHTML('file://' + hello_file_path,
                                           dbus_interface=manager_interface)

    bs_file = BeautifulSoup(hello_file.read())
    bs_html = BeautifulSoup(hello_html)
    self.assertEqual(bs_file.body, bs_html.body)

if __name__ == '__main__':
  unittest.main()
