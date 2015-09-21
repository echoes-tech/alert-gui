class echoes_alert_gui (
  $branch          = $echoes_alert_gui::params::branch,
  $cert_file       = $echoes_alert_gui::params::cert_file,
  $install_dir     = $echoes_alert_gui::params::install_dir,
  $http_port       = $echoes_alert_gui::params::http_port,
  $https           = $echoes_alert_gui::params::https,
  $https_port      = $echoes_alert_gui::params::https_port,
  $manage_firewall = $echoes_alert_gui::params::manage_firewall,
  $priv_key_file   = $echoes_alert_gui::params::priv_key_file,
  $servername      = $echoes_alert_gui::params::servername,
  $service_enable  = $echoes_alert_gui::params::service_enable,
  $service_ensure  = $echoes_alert_gui::params::service_ensure,
  $service_name    = $echoes_alert_gui::params::service_name,
  $version         = $echoes_alert_gui::params::version,
) inherits echoes_alert_gui::params {
  validate_string($branch)
  validate_absolute_path($install_dir)
  validate_integer($http_port, 65535, 1)
  validate_bool($https)
  if $https {
    validate_absolute_path($cert_file)
    validate_integer($https_port, 65535, 1)
    validate_absolute_path($priv_key_file)
  }
  validate_bool($manage_firewall)
  validate_bool($service_enable)
  validate_string($service_ensure)
  validate_string($service_name)
  validate_string($servername)
  validate_string($version)

  require echoes_alert_dbo

  anchor { "${module_name}::begin": } ->
  class { "${module_name}::install": } ->
  class { "${module_name}::service": } ->
  class { "${module_name}::firewall": } ->
  anchor { "${module_name}::end": }
}
