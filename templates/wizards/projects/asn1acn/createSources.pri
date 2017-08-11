defineReplace(filterASN1ACNFiles) {
    allFiles = $${1}

    for(file, allFiles) {
        splitted = $$split(file, ".")
        extension = $$last(splitted)

        equals(extension, asn)|equals(extension, asn1)|equals(extension, acn) {
            fileNames += $$file
        }
    }

    return($$fileNames)
}

ASN1ACNFILES = $${filterASN1ACNFiles($${DISTFILES})}

prepare.target += prepare
prepare.commands += $$sprintf($$QMAKE_MKDIR_CMD, $$ASN1SCC_PRODUCTS_DIR)
QMAKE_EXTRA_TARGETS += prepare
PRE_TARGETDEPS += prepare

asn1scc.name = ASN1SCC
asn1scc.input = ASN1ACNFILES
asn1scc.output = asn1scc.out
asn1scc.commands = $$ASN1SCC -c ${QMAKE_FILE_IN} -o $$ASN1SCC_PRODUCTS_DIR
asn1scc.variable_out = ASN1ACN_SOURCES
asn1scc.depends += prepare
asn1scc.CONFIG += combine target_predeps no_link
QMAKE_EXTRA_COMPILERS += asn1scc

cleanSources.target += cleanSources
cleanSources.commands += $$QMAKE_DEL_TREE $$ASN1SCC_PRODUCTS_DIR
clean.depends += cleanSources
QMAKE_EXTRA_TARGETS += cleanSources clean
