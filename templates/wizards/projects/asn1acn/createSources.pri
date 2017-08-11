defineReplace(filterASN1ACNFiles) {
    allFiles = $${1}

    for(file, allFiles) {
        splitted = $$split(file, ".")
        extension = $$last(splitted)

        equals(extension, asn)|equals(extension, asn1)|equals(extension, acn) {
            fileNames += $${PWD}/$${file}
        }
    }

    return($$fileNames)
}

ASN1ACNFILES = $$filterASN1ACNFiles($${DISTFILES})

prepare.target += prepare
prepare.commands += $$sprintf($$QMAKE_MKDIR_CMD, $$ASN1SCC_PRODUCTS_DIR)

runAsn1Scc.target += runAsn1Scc
runAsn1Scc.commands += $$ASN1SCC -c $$ASN1ACNFILES -o $$ASN1SCC_PRODUCTS_DIR
runAsn1Scc.depends += prepare

cleanSources.target += cleanSources
cleanSources.commands += $$QMAKE_DEL_TREE $$ASN1SCC_PRODUCTS_DIR
clean.depends += cleanSources

QMAKE_EXTRA_TARGETS += prepare runAsn1Scc cleanSources clean
PRE_TARGETDEPS += prepare runAsn1Scc
