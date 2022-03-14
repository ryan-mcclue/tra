// SPDX-License-Identifier: zlib-acknowledgement

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <dbus/dbus.h>

int
main(int argc, char *argv[])
{
  // get first available device
  int dev_id = hci_get_route(NULL);
  if (dev_id < 0) return 0;

  struct hci_dev_info dev_info;
  if (hci_devinfo(dev_id, &dev_info) < 0)
  {
    // error
  }

  printf("%s\n", dev_info.name);

  //DBusError dbus_error;
  //dbus_error_init(&dbus_error);

  //DBusConnection *connection = dbus_bus_get(DBUS_BUS_SESSION, &dbus_error); 
  //if (dbus_error_is_set(&dbus_error))
  //{
  //  fprintf(stderr, "error: %s\n", dbus_error.message);
  //}

  //int ret = dbus_bus_request_name(connection, "my.bluetooth.client",
  //    DBUS_NAME_FLAG_REPLACE_EXISTING, &dbus_error);


  return 0;
}

// dbus built on-top of low-level IPCs like shared memory and sockets
// system bus for kernel, i.e for all users
// session bus for user
// if well known name of the service (org.freedesktop.ModemManager) is not started, dbus will start it for us?
// service exposes objects (/org/freedesktop/ModemManager)
// objects can have various interfaces (org.freedesktop.DBus.ObjectManager)
// the interface will have methods to access data (GetManagedObjects()), also have properties or signals

// our process interacts with dbus-daemon (libdbus) which interacts with systemd (sd-bus)
