# Private class
class echoes_alert_gui::install inherits echoes_alert_gui {
  if $caller_module_name != $module_name {
    fail("Use of private class ${name} by ${caller_module_name}")
  }

  if $echoes_alert_gui::https {
    require openssl
  }

  file { $echoes_alert_gui::install_dir:
    ensure => 'directory',
    owner  => 0,
    group  => 0,
    mode   => '0755'
  }->
  file { $echoes_alert_gui::binary_path:
    ensure => 'directory',
    owner  => 0,
    group  => 0,
    mode   => '0755'
  }->
  file { "${echoes_alert_gui::binary_path}/${echoes_alert_gui::service_name}":
    ensure => 'file',
    owner  => 0,
    group  => 0,
    mode   => '0755',
    source => "puppet:///modules/${module_name}/${echoes_alert_gui::branch}/${echoes_alert_gui::version}/gui",
  }

  file { "${echoes_alert_gui::install_dir}/images":
    ensure  => 'directory',
    owner   => 0,
    group   => 0,
    mode    => '0644',
    recurse => true,
    purge   => true,
    source  => "puppet:///modules/${module_name}/${branch}/${version}/images",
  }->
  file { "${echoes_alert_gui::install_dir}/resources":
    ensure  => 'directory',
    owner   => 0,
    group   => 0,
    mode    => '0644',
    recurse => true,
    purge   => true,
    source  => "puppet:///modules/${module_name}/${branch}/${version}/resources",
  }->
  file { "${echoes_alert_gui::install_dir}/favicon.ico":
    ensure => 'file',
    owner  => 0,
    group  => 0,
    mode   => '0644',
    source => "puppet:///modules/${module_name}/${branch}/${version}/favicon.ico",
  }->

}
