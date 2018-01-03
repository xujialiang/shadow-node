#include "iotjs_def.h"
#include <dbus/dbus.h>

/**
 * Watcher Functions
 */
static dbus_bool_t watch_add(DBusWatch* watch, void* data) {
  if (!dbus_watch_get_enabled(watch) || 
    dbus_watch_get_data(watch) != NULL) {
    return true;
  }
  fprintf(stdout, "%s\n", "add watcher");
  dbus_watch_set_data(watch, NULL, NULL);
  return true;
}

static void watch_remove(DBusWatch* watch, void* data) {
  fprintf(stdout, "%s\n", "remove watcher");
  dbus_watch_set_data(watch, NULL, NULL);
}

static void watch_handle(DBusWatch* watch, void* data) {
  fprintf(stdout, "%s\n", "handling watcher");
}

/**
 * Timeout Functions
 */
static dbus_bool_t timeout_add(DBusTimeout* timeout, void* data) {
  fprintf(stdout, "%s\n", "add timeout");
  return true;
}

static void timeout_remove(DBusTimeout* timeout, void* data) {
  fprintf(stdout, "%s\n", "remove timeout");
}

static void timeout_toggled(DBusTimeout* timeout, void* data) {
  fprintf(stdout, "%s\n", "toggle timeout");
}

/**
 * Wakeup Callbacks
 */
static void conn_wakeup(void* data) {
  fprintf(stdout, "%s\n", "connection wakeup");
}

static void conn_close_cb(void* data) {
  fprintf(stdout, "%s\n", "connection closed");
}

static void conn_free_cb(void* data) {
  // TODO
}

static const jerry_object_native_info_t conn_type_info =
{
  .free_cb = conn_free_cb
};

JS_FUNCTION(GetBus) {
  DBusConnection *conn = NULL;
  DBusError error;
  dbus_error_init(&error);

  DJS_CHECK_ARGS(1, number);
  int type = JS_GET_ARG(0, number);

  if (type == 0/* BUS_SYSTEM */) {
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
  } else if (type == 1/* BUS_SESSION */) {
    conn = dbus_bus_get(DBUS_BUS_SESSION, &error);
  }

  if (conn == NULL) {
    if (dbus_error_is_set(&error)) {
      fprintf(stderr, "%s\n", error.message);
      return jerry_create_undefined();
    } else {
      fprintf(stderr, "%s\n", "failed to get bus object");
      return jerry_create_undefined();
    }
  }

  dbus_connection_set_exit_on_disconnect(conn, false);
  dbus_connection_set_watch_functions(conn,
    watch_add, watch_remove, watch_handle, NULL, NULL);
  dbus_connection_set_timeout_functions(conn,
    timeout_add, timeout_remove, timeout_toggled, NULL, NULL);
  dbus_connection_set_wakeup_main_function(conn,
    conn_wakeup, NULL, conn_close_cb);
  // dbus_connection_add_filter(conn,);

  jerry_value_t jbus = jerry_create_object();
  jerry_set_object_native_pointer(jbus, conn, &conn_type_info);
  return jerry_create_undefined();
}

JS_FUNCTION(ReleaseBus) {
  // TODO
  return jerry_create_undefined();
}

JS_FUNCTION(CallMethod) {
  // TODO
  return jerry_create_undefined();
}

JS_FUNCTION(RequestName) {
  // TODO
  return jerry_create_undefined();
}

JS_FUNCTION(RegisterObjectPath) {
  // TODO
  return jerry_create_undefined();
}

JS_FUNCTION(UnregisterObjectPath) {
  // TODO
  return jerry_create_undefined();
}

jerry_value_t InitDBus() {
  jerry_value_t dbus = jerry_create_object();

  iotjs_jval_set_method(dbus, "getBus", GetBus);
  iotjs_jval_set_method(dbus, "releaseBus", ReleaseBus);
  iotjs_jval_set_method(dbus, "callMethod", CallMethod);
  iotjs_jval_set_method(dbus, "requestName", RequestName);
  iotjs_jval_set_method(dbus, "registerObjectPath", RegisterObjectPath);
  iotjs_jval_set_method(dbus, "unregisterObjectPath", UnregisterObjectPath);
  // iotjs_jval_set_method(dbus, "sendMessageReply", SendMessageReply);
  // iotjs_jval_set_method(dbus, "sendErrorMessageReply", SendErrorMessageReply);
  // iotjs_jval_set_method(dbus, "setObjectHandler", SetObjectHandler);
  // iotjs_jval_set_method(dbus, "parseIntrospectSource", ParseIntrospectSource);
  // iotjs_jval_set_method(dbus, "setSignalHandler", SetSignalHandler);
  // iotjs_jval_set_method(dbus, "addSignalFilter", AddSignalFilter);
  // iotjs_jval_set_method(dbus, "setMaxMessageSize", SetMaxMessageSize);
  // iotjs_jval_set_method(dbus, "emitSignal", EmitSignal);

  return dbus;
}