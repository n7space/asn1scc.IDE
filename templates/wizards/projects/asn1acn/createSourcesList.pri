defineReplace(createFileNames) {
    allFiles = $${1}

    for(file, allFiles) {
        splitted = $$split($$basename(file), ".")
        extension = $$last(splitted)

        equals(extension, asn)|equals(extension, asn1) {
            fileNames += $$first(splitted)
        }
    }

    return($$fileNames)
}

defineReplace(createHeadersList) {
    headersNames = $${1}

    for(name, headersNames) {
        header = $${ASN1SCC_PRODUCTS_DIR}/$${name}.h
        headers += $$header
    }

    return($$headers)
}

defineReplace(createSourcesList) {
    sourcesNames = $${1}

    for(name, sourcesNames) {
        source = $${ASN1SCC_PRODUCTS_DIR}/$${name}.c
        sources += $$source
    }

    return($$sources)
}

names = $$createFileNames($$DISTFILES)

!isEmpty(names) {
    PERSISTENT_HEADERS = $${ASN1SCC_PRODUCTS_DIR}/asn1crt.h
    PERSISTENT_SOURCES = $${ASN1SCC_PRODUCTS_DIR}/acn.c $${ASN1SCC_PRODUCTS_DIR}/asn1crt.c $${ASN1SCC_PRODUCTS_DIR}/real.c
}

SOURCES += $$createSourcesList($$names) $$PERSISTENT_SOURCES
HEADERS += $$createHeadersList($$names) $$PERSISTENT_HEADERS
