# Features
Plugin extends existing Qt Creator's features to support ASN.1 and ACN files and models.

## Code highlighting
ASN.1 and ACN syntax is now highlighted in editor.
![Screen-Highlight](images/screen-highlight.png)

## Code completion
Code auto-completion (`{} ''` etc.) and basic completion suggestions are available for all ASN.1 and ACN files. 
Suggestions based on existing models requires working ASN1SCC and files need to be in proper project.
![Screen-Completion](images/screen-completion.png)

## Code snippets
Code snippets can be used to speed up typing.
![Screen-Snippets](images/screen-snippets.png)

## Error highlighting
Errors reported by ASN1SCC are highlighted - both during typing and after compilation.
![Screen-Error](images/screen-error.png)

## New project type
QMake and CMake projects with ASN.1/ACN models can be created. ASN.1/ACN files are used to generate C code, included in project itself.
![Screen-New-Project](images/screen-new-project.png)

## File outline
ASN.1 files included in project can have proper Outline view displayed.
![Screen-Outline](images/screen-outline.png)

## ASN.1 Types View
All models included in project can be displayed in single view.
![Screen-Types-View](images/screen-types-view.png)

## Code navigation
User can easily switch between ASN.1 and ACN files (`F4` by default).
Type usage can also be easily followed to it's ASN.1 definition (`F2` by default).
![Screen-Follow](images/screen-follow.png)

## Model usages
All usages of model in project can be found (`Ctrl+Shift+U` by default).
![Screen-Usages](images/screen-usages.png)

## ASN.1 Components libraries
Models can be imported from ASN.1 components libraries. Users can define their own libraries.
Plugin is distributed with built-in [PUS-C library](https://github.com/n7space/asn1-pusc-lib).
![Screen-Import](images/screen-import.png)