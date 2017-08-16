TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

%{JS: if ('%{AddAsnFile}' === 'true' && '%{AddAcnFile}' === 'true')
        'DISTFILES += \\\\ \n %{AsnFullFileName} \\\\ \n %{AcnFullFileName}'
      else if ('%{AddAsnFile}' === 'true' )
        'DISTFILES += \\\\ \n %{AsnFullFileName}'
      else if ('%{AddAcnFile}' === 'true')
        'DISTFILES += \\\\ \n %{AcnFullFileName}'
      else
        'DISTFILES += ' }

include(handleAsn1AcnBuild.pri)
