# asn1scc.IDE
Qt Creator plugin for asn1scc - ASN.1/ACN compiler for embedded systems

[![Build Status](https://travis-ci.org/n7space/asn1scc.IDE.svg?branch=master)](https://travis-ci.org/n7space/asn1scc.IDE)
[![Build status](https://ci.appveyor.com/api/projects/status/4vtpq8py5dc53s7b/branch/master?svg=true)](https://ci.appveyor.com/project/hcorg/asn1scc-ide/branch/master)

### [Project Home Page](https://n7space.github.io/asn1scc.IDE/)

## Installation
 0. Install desired version of [QtCreator](https://www.qt.io/download), if you don't have one already
 1. Download plugin [release](https://github.com/n7space/asn1scc.IDE/releases) corresponding to installed QtCreator version (plugin's version is based on compatible QtCreator's version, so, for example, plugin 4.5.0 should work with QtCreator 4.5.1, but for QtCreator 4.6.0 plugin 4.6.x is required)
 2. Unpack release package contents into QtCreator installation directory:
    * `/opt/Qt/Tools/QtCreator/` on Linux  
    * `C:\Qt\Tools\QtCreator` on Windows   
 3. Run QtCreator - plugin should be available on plugins list in *Help > About Plugins...*
 
## ASN1SCC requirements
Most features of plugin depends on properly working [ASN1SCC](https://github.com/ttsiodras/asn1scc) compiler. It is distributed along with plugin itself, or plugin can be configured to use different one. To run compiler system requires:
  * `mono` package on Linux
  * .NET Framework 4.6 on Windows (preinstalled in Windows 10 and newer)
  
## Main features
 * Support for ASN.1 files in QMake and CMake projects
 * Outline view for ASN.1 files
 * ASN.1 Types view for all models in project
 * 'Find Usages' for ASN.1 types
