**asn1scc.IDE** is a [Qt Creator](https://www.qt.io/download) plugin for [ASN1SCC](https://github.com/ttsiodras/asn1scc) - ASN.1/ACN compiler for embedded systems.

Plugin was developed by [N7 Space](http://www.n7space.com) under a programme and funded by European Space Agency ([ESA](http://www.esa.int)) and is distributed under [GPL v3.0](https://www.gnu.org/licenses/gpl-3.0.html) license.

## Installation
 0. Install desired version of [Qt Creator](https://www.qt.io/download), if you don't have one already
 1. Download plugin [release](https://github.com/n7space/asn1scc.IDE/releases) corresponding to installed QtCreator version (plugin's version is based on compatible QtCreator's version, so, for example, plugin 4.5.0 should work with QtCreator 4.5.1, but for QtCreator 4.6.0 plugin 4.6.x is required)
 2. Unpack release package contents into QtCreator installation directory:
    * `/opt/Qt/Tools/QtCreator/` on Linux  
    * `C:\Qt\Tools\QtCreator` on Windows   
 3. Run QtCreator - plugin should be available on plugins list in *Help > About Plugins...* ![Screen-About](images/screen-about.png)
 
## ASN1SCC requirements
Most features of plugin depends on properly working [ASN1SCC](https://github.com/ttsiodras/asn1scc) compiler. It is distributed along with plugin itself, or plugin can be configured to use different one. To run compiler system requires:
  * `mono` package on Linux
  * .NET Framework 4.6 on Windows (preinstalled in Windows 10 and newer)
  
## Main features
 * Support for ASN.1 files in QMake and CMake projects
 * Outline view for ASN.1 files
 * ASN.1 Types view for all models in project
 * 'Find Usages' for ASN.1 types
