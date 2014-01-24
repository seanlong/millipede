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

  def test_set_mode_pass(self):
    self._manager_obj.SetMode('auto', dbus_interface=manager_interface)

  def test_set_mode_fail(self):
    try:
      self._manager_obj.SetMode(1, dbus_interface=manager_interface)
    except Exception as e:
      self.assertEqual(e.get_dbus_name(), self.error_name)


class TestClawerManagerAuto(unittest.TestCase):
  manager_auto_if = 'org.seanlong.ClawerDriver.Manager.Auto'
  manager_auto_error = 'org.seanlong.ClawerDriver.ManagerAuto.Error'

  def setUp(self):
    self._manager_obj = connect_manager()
    self._manager_obj.SetMode('auto', dbus_interface=manager_interface)

  def test_set_pool_size(self):
    self._manager_obj.SetPoolSize(10, dbus_interface=self.manager_auto_if)

  def test_get_html_http(self):
    self._manager_obj.SetPoolSize(-1, dbus_interface=self.manager_auto_if)
    ret = self._manager_obj.GetHTML('http://www.baidu.com',
                                    dbus_interface=self.manager_auto_if)
    self.assertTrue(len(ret) > 0)

  def test_get_html_file(self):
    self._manager_obj.SetPoolSize(-1, dbus_interface=self.manager_auto_if)
    hello_file_path = os.getcwd() + '/data/hello.html'  
    hello_file = open(hello_file_path, 'r')
    hello_html = self._manager_obj.GetHTML('file://' + hello_file_path,
                                           dbus_interface=self.manager_auto_if)

    bs_file = BeautifulSoup(hello_file.read())
    bs_html = BeautifulSoup(hello_html)
    self.assertEqual(bs_file.body, bs_html.body)


if __name__ == '__main__':
  unittest.main()
