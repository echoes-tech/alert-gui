# == Class: echoes_alert_gui::params
#
# This is a container class with default parameters for echoes_alert_gui classes.
class echoes_alert_gui::params {
  $name = 'gui'

  $branch          = 'master'
  $cert_file       = undef
  $install_dir     = "/opt/echoes-alert/${name}"
  $http_port       = 80
  $https           = false
  $https_port      = 443
  $manage_firewall = false
  $priv_key_file   = undef
  $servername      = "echoes-alert-${name}"
  $service_enable  = true
  $service_ensure  = 'running'
  $service_name    = "ea-${name}"
  $version         = 'latest'

  $binary_path = "${install_dir}/bin"
}
