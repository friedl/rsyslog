**Title:** rsyslog service was huped repeatedly #4838

**Summary:**
The user reports rsyslog being HUPed daily, likely due to standard log rotation. They seek assistance in understanding or stopping this recurring event.

**Analysis:**
This is not a bug or missing feature. SIGHUP is the standard signal used to notify rsyslog of log rotation. rsyslog logs "rsyslogd was HUPed" to indicate it has successfully reloaded its output files.

**Solution:**
The "rsyslogd was HUPed" message is a normal informational event, typically triggered by your system's log rotation utility (such as `logrotate`). When `logrotate` moves a log file, it sends a SIGHUP to rsyslog to ensure it closes the old file handle and opens the new one. Seeing this once a day usually matches a daily log rotation schedule.

If you wish to suppress these messages from your logs, you can use the following RainerScript configuration in your global settings:

```rainerscript
global(internalmsg.severity="warning")
```

*Note: This parameter is available in rsyslog 8.1905.0 and later. It prevents rsyslog from logging internal messages with a severity lower than "warning" (the HUP message is logged at the "info" level).*

If you are using an older version (like 8.24.0 on RHEL 7) and the above RainerScript does not work, you can use the legacy directive:

```syslog
$LogRSyslogStatusMessages off
```

To confirm what is sending the signal, you can check your `logrotate` configuration, typically found in `/etc/logrotate.d/syslog` or `/etc/logrotate.conf`. Look for a `postrotate` script that calls `kill -HUP` or `systemctl reload rsyslog`.
