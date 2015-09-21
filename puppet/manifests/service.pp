# Private class
class echoes_alert_gui::service inherits echoes_alert_gui {
  if $caller_module_name != $module_name {
    fail("Use of private class ${name} by ${caller_module_name}")
  }

  $monit_file = "/etc/monit/conf.d/${echoes_alert_gui::service_name}"

  wt::service { $echoes_alert_gui::service_name:
    binary_path    => $echoes_alert_gui::binary_path,
    cert_file      => $echoes_alert_gui::cert_file,
    http_port      => $echoes_alert_gui::http_port,
    https          => $echoes_alert_gui::https,
    https_port     => $echoes_alert_gui::https_port,
    priv_key_file  => $echoes_alert_gui::priv_key_file,
    service_enable => $echoes_alert_gui::service_enable,
    service_ensure => $echoes_alert_gui::service_ensure,
    service_name   => $echoes_alert_gui::service_name,
  }->
  monit::check { $echoes_alert_gui::service_name:
    content => template("${module_name}/${echoes_alert_gui::branch}/${echoes_alert_gui::version}${monit_file}.erb"),
  }

}
