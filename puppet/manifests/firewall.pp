# Private class
class echoes_alert_gui::firewall inherits echoes_alert_gui {
  if $caller_module_name != $module_name {
    fail("Use of private class ${name} by ${caller_module_name}")
  }

  if $echoes_alert_gui::manage_firewall {
    if defined('::firewall') {
      if $echoes_alert_gui::https {
        firewall { '101 allow GUI HTTPs access':
          port  => [ $echoes_alert_gui::https_port ],
          proto => 'tcp',
          jump  => 'allowed',
        }->
        firewall { '101 allow GUI HTTPs access - IPv6':
          port     => [ $echoes_alert_gui::https_port ],
          proto    => 'tcp',
          jump     => 'allowed',
          provider => 'ip6tables',
        }
      }

      firewall { '100 allow GUI HTTP access':
        port  => [ $echoes_alert_gui::http_port ],
        proto => 'tcp',
        jump  => 'allowed',
      }->
      firewall { '100 allow GUI HTTP access - IPv6':
        port     => [ $echoes_alert_gui::http_port ],
        proto    => 'tcp',
        jump     => 'allowed',
        provider => 'ip6tables',
      }
    }
  }
}
