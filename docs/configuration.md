# Configuration

The plugin adds it's own set of parameters to Qt Creator's options (*Tools* -> *Options*).
Options are divided into categories, each is described below.

## Libraries
This option page list ASN.1 components libraries available for user as models' import sources.
![Options-Libraries](images/options-libraries.png)

Library is a folder tree containing ASN.1 and ACN files and optionaly additional JSON metadata.

Auto-detected libraries are the ones found in Qt Creator's installation directory:
 * default `/opt/Qt/Tools/QtCreator/share/qtcreator/asn1acn/libs` on Linux
 * default `C:\Qt\Tools\QtCreator\share\qtcreator\asn1acn\libs` on Windows

or in user's local settings directories:
 * `~/.config/QtProject/qtcreator/asn1acn/libs` on Linux
 * `%APPDATA%\QtProject\qtcreator\asn1acn\libs` on Windows
 
User can add folders using `Add` button, which will be listed in 'Manual' list.

## asn1scc.Fuzzer
This page configures path to [asn1scc.Fuzzer](https://github.com/n7space/asn1scc.Fuzzer) tool, which can be used to generate test cases based on ASN.1/ACN models.

## Service
The plugin relies on [ASN1SCC](https://github.com/ttsiodras/asn1scc) compiler to process ASN.1 files.
The plugin executes ASN1SCC in 'daemon' mode and communicates with it using local sockets.
Usually the default settings should work for most users, as by default the plugin executes the compiler distributed with it, but user might want to use other version of the compiler. Also socket-based communication might cause problems on some setups, so options on this page might need some tweaking (see [Troubleshooting](troubleshooting.html)).
![Options-Service](images/options-service.png)

 * Service path - path to ASN1SCC Daemon executable.
 * Listening URI - URI on which the daemon should listen for the plugin requests, might need change if the default address is blocked for any reason.
 * Stay alive period - the daemon will quit if no messages from the plugin are recevied in provided period; used mostly for debugging.
 * Disable watchdog - the daemon will no longer be restarted due to lack of communication.
