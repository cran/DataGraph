//
//  DTTable.cpp
//  Chromosomes
//
//  Created by David Adalsteinsson on 4/2/17.
//
//

#include "DTTable.h"
#include "DTUtilities.h"
#include "DTArrayConversion.h"

// http://docs.rexamine.com/R-devel/Rinternals_8h_source.html

#pragma mark Table Column Structure

    std::string name;
    std::string type; // Number, Text, Date, Table
    DTPointer<DTTableStructure> tableStructure;
    
bool DTTableColumnStructure::operator!=(const DTTableColumnStructure &v) const
{
    return (!this->operator==(v));
}

bool DTTableColumnStructure::operator==(const DTTableColumnStructure &v) const
{
    if (name!=v.name) return false;
    if (type!=v.type) return false;
    if (type=="Table")
        return (*tableStructure==*v.tableStructure);

    return true;
}

#pragma mark Table Column

DTTableColumn DTTableColumn::TextColumn(const std::string &nm,const DTIntArray &a,const DTCharArray &m)
{
    DTTableColumn toReturn(nm);
    toReturn.type = DTTableColumn::IndexedText;
    toReturn.intVersion = a;
    toReturn.charVersion = m;
    return toReturn;
}

DTTableColumn DTTableColumn::NumberColumn(const std::string &nm,const DTIntArray &a,const DTCharArray &m)
{
    DTTableColumn toReturn(nm);
    toReturn.type = DTTableColumn::Int;
    toReturn.intVersion = a;
    toReturn.mask = m;
    return toReturn;
}

DTTableColumn DTTableColumn::NumberColumn(const std::string &nm,const DTIntArray &a)
{
    DTTableColumn toReturn(nm);
    toReturn.type = DTTableColumn::Int;
    toReturn.intVersion = a;
    return toReturn;
}

DTTableColumn DTTableColumn::NumberColumn(const std::string &nm,const DTDoubleArray &a,const DTCharArray &m)
{
    DTTableColumn toReturn(nm);
    toReturn.type = DTTableColumn::Double;
    toReturn.doubleVersion = a;
    toReturn.mask = m;
    return toReturn;
}

DTTableColumn DTTableColumn::NumberColumn(const std::string &nm,const DTDoubleArray &a)
{
    DTTableColumn toReturn(nm);
    toReturn.type = DTTableColumn::Double;
    toReturn.doubleVersion = a;
    return toReturn;
}

DTTableColumn DTTableColumn::NumberColumn(const std::string &nm,const DTShortIntArray &a,const DTCharArray &m)
{
    DTTableColumn toReturn(nm);
    toReturn.type = DTTableColumn::Short;
    toReturn.shortVersion = a;
    toReturn.mask = m;
    return toReturn;
}

DTTableColumn DTTableColumn::NumberColumn(const std::string &nm,const DTShortIntArray &a)
{
    DTTableColumn toReturn(nm);
    toReturn.type = DTTableColumn::Short;
    toReturn.shortVersion = a;
    return toReturn;
}

DTTableColumn DTTableColumn::NumberColumn(const std::string &nm,const DTCharArray &a,const DTCharArray &m)
{
    DTTableColumn toReturn(nm);
    toReturn.type = DTTableColumn::Character;
    toReturn.charVersion= a;
    toReturn.mask = m;
    return toReturn;
}

DTTableColumn DTTableColumn::NumberColumn(const std::string &nm,const DTCharArray &a)
{
    DTTableColumn toReturn(nm);
    toReturn.type = DTTableColumn::Character;
    toReturn.charVersion = a;
    return toReturn;
}

DTTableColumn DTTableColumn::DateColumn(const std::string &nm,const DTDoubleArray &a,const DTCharArray &m)
{
    DTTableColumn toReturn(nm);
    toReturn.type = DTTableColumn::Date;
    toReturn.doubleVersion = a;
    toReturn.mask = m;
    return toReturn;
}

DTTableColumn DTTableColumn::DateColumn(const std::string &nm,const DTDoubleArray &a)
{
    DTTableColumn toReturn(nm);
    toReturn.type = DTTableColumn::Date;
    toReturn.doubleVersion = a;
    return toReturn;
}

DTTableColumn DTTableColumn::TextColumn(const std::string &nm,const DTStringList &a)
{
    DTTableColumn toReturn(nm);
    toReturn.type = DTTableColumn::Text;
    toReturn.stringVersion = a;
    return toReturn;
}

DTTableColumn DTTableColumn::TableColumn(const std::string &nm,const DTTable &t)
{
    DTTableColumn toReturn(nm);
    toReturn.type = DTTableColumn::Table;
    toReturn.table = DTPointer<DTTable>(new DTTable(t));
    return toReturn;
}

DTTableColumn DTTableColumn::ExtractRows(const DTRange &r) const
{
    DTRange final = Intersection(r,DTRange(0,NumberOfRows()));
    DTTableColumn toReturn(name);
    toReturn.type = type;
    if (final.length==0) return toReturn;
    
    switch (type) {
        case Empty:
            break;
        case Double:
        case Date:
            toReturn.doubleVersion = ExtractIndices(doubleVersion,final);
            break;
        case Float:
            toReturn.floatVersion = ExtractIndices(floatVersion,final);
            break;
        case Int:
            toReturn.intVersion = ExtractIndices(intVersion,final);
            break;
        case Short:
            toReturn.shortVersion = ExtractIndices(shortVersion,final);
            break;
        case Character:
            toReturn.charVersion = ExtractIndices(charVersion,final);
            break;
        case IndexedText:
            DTErrorMessage("DTTableColumn::ExtractRows","Not done yet - IndexedText");
            break;
        case Text:
            DTErrorMessage("DTTableColumn::ExtractRows","Not done yet - Text");
            break;
        case Table:
            DTErrorMessage("DTTableColumn::ExtractRows","Not done yet - Table");
            break;
    }

    return toReturn;
}

int DTTableColumn::NumberOfRows(void) const
{
    int toReturn = 0;
    switch (type) {
        case Empty:
            break;
        case Double:
        case Date:
            toReturn = doubleVersion.Length();
            break;
        case Float:
            break;
        case Int:
            break;
        case Short:
            break;
        case Character:
            break;
        case IndexedText:
            toReturn = intVersion.Length();
            break;
        case Text:
            toReturn = stringVersion.NumberOfStrings();
            break;
        case Table:
            return (table ? table->NumberOfRows() : 0);
            break;
    }
    return toReturn;
}

DTTableColumn DTTableColumn::ChangeName(const std::string &nm) const
{
    DTTableColumn toReturn = *this;
    toReturn.name = nm;
    return toReturn;
}

DTDoubleArray DTTableColumn::NumberArray(void) const
{
    DTDoubleArray toReturn;
    switch (type) {
        case Empty:
            break;
        case Double:
            toReturn = doubleVersion;
            break;
        case Date:
            DTErrorMessage("TableColumn::NumberArray","This is a date column");
            break;
        case Float:
            toReturn = ConvertToDouble(floatVersion);
            break;
        case Int:
            toReturn = ConvertToDouble(intVersion);
            break;
        case Short:
            toReturn = ConvertToDouble(shortVersion);
            break;
        case Character:
            toReturn = ConvertToDouble(charVersion);
            break;
        case IndexedText:
        case Text:
            DTErrorMessage("TableColumn::NumberArray","This is a text column");
            break;
        case Table:
            DTErrorMessage("TableColumn::NumberArray","This is a table");
            break;
    }
    return toReturn;
}

DTTableColumnStructure DTTableColumn::Structure(void) const
{
    DTTableColumnStructure toReturn;
    toReturn.name = name;
    
    switch (type) {
        case Empty:
            toReturn.type = "Empty";
            break;
        case Double:
        case Float:
        case Int:
        case Short:
        case Character:
            toReturn.type = "Number";
            break;
        case Date:
            toReturn.type = "Date";
            break;
        case IndexedText:
        case Text:
            toReturn.type = "Text";
            break;
        case Table:
            toReturn.type = "Table";
            toReturn.tableStructure = DTPointer<DTTableStructure>(new DTTableStructure(table->Structure()));
            break;
    }

    return toReturn;
}

void DTTableColumn::pinfoWithIndent(const std::string &indent) const
{
#ifndef DG_NOSTDErrOut
    std::string padding = ".................................";

    std::string desc = name + " ";
    if (desc.length()<30) desc = desc + string(padding,0,30-desc.length());
    if (desc.length()>30) desc = string(desc,0,30);
    
    std::cerr << indent << desc << " - ";
    
    switch (type) {
        case Empty:
            std::cerr << " empty - ";
            break;
        case Double:
            std::cerr << "double - " << doubleVersion.Length() << " rows";
            break;
        case Date:
            std::cerr << "  date - " << doubleVersion.Length() << " rows";
            break;
        case Float:
            std::cerr << " float - " << floatVersion.Length() << " rows";
            break;
        case Int:
            std::cerr << "   int - " << intVersion.Length() << " rows";
            break;
        case Short:
            std::cerr << " short - " << shortVersion.Length() << " rows";
            break;
        case Character:
            std::cerr << "  char - " << charVersion.Length() << " rows";
            break;
        case IndexedText:
            std::cerr << "Text (ind) - " << intVersion.Length() << " rows";
            break;
        case Text:
            std::cerr << "  Text - ";
            break;
        case Table:
            std::cerr << " Table - \n";
            table->pinfoWithIndent(indent+"   ");
            break;
    }
    
//    if (maskDefined) {
//        std::cerr << " some entries are missing";
//    }
    
    if (type!=Table) {
        std::cerr << std::endl;
    }
#endif
}

DTTableColumn DTTableColumn::Read(const DTDataStorage &input,const std::string &name)
{
    std::string nm = input.ReadString(name+"N");
    std::string varName = name+"V";
    std::string type = input.ReadString(varName+"_T");
    
    bool maskDefined = false;
    DTCharArray mask;
    if (input.Contains(varName+"_mask")) {
        mask = input.ReadCharArray(varName+"_mask");
    }
    
    if (type=="") {
        return DTTableColumn(); // Empty
    }
    else if (type=="Number") {
        if (input.SavedAsDouble(varName)) {
            DTDoubleArray da = input.ReadDoubleArray(varName);
            if (maskDefined)
                return DTTableColumn::NumberColumn(nm,da,mask);
            else
                return DTTableColumn::NumberColumn(nm,da);
        }
        else if (input.SavedAsCharacter(varName)) {
            DTCharArray ca = input.ReadCharArray(varName);
            if (maskDefined)
                return DTTableColumn::NumberColumn(nm,ca,mask);
            else
                return DTTableColumn::NumberColumn(nm,ca);
        }
        else if (input.SavedAsInt(varName)) {
            DTIntArray sa = input.ReadIntArray(varName);
            if (maskDefined)
                return DTTableColumn::NumberColumn(nm,sa,mask);
            else
                return DTTableColumn::NumberColumn(nm,sa);
        }
        else if (input.SavedAsShort(varName)) {
            DTShortIntArray sa = input.ReadShortIntArray(varName);
            if (maskDefined)
                return DTTableColumn::NumberColumn(nm,sa,mask);
            else
                return DTTableColumn::NumberColumn(nm,sa);
        }
        else {
            // Fallback
            DTDoubleArray da = input.ReadDoubleArray(varName);
            if (maskDefined)
                return DTTableColumn::NumberColumn(nm,da,mask);
            else
                return DTTableColumn::NumberColumn(nm,da);
        }
    }
    else if (type=="Indexed") {
        DTIntArray ind = input.ReadIntArray(varName);
        DTCharArray text = input.ReadCharArray(varName+"_S");
        return DTTableColumn::TextColumn(nm,ind,text);
    }
    else if (type=="Date") {
        DTDoubleArray da = input.ReadDoubleArray(varName);
        if (maskDefined)
            return DTTableColumn::DateColumn(nm,da,mask);
        else
            return DTTableColumn::DateColumn(nm,da);
    }
    else if (type=="Table") {
        DTTable subTable;
        ::Read(input,varName,subTable);
        return DTTableColumn::TableColumn(nm,subTable);
    }
    else {
        DTErrorMessage("Not done");
    }

    return DTTableColumn(nm);
}

void DTTableColumn::WriteStructure(DTDataStorage &output,const std::string &saveAs) const
{
    output.Save(name,saveAs+"N");
    switch (type) {
        case Empty:
            output.Save("",saveAs+"T");
            break;
        case Double:
        case Float:
        case Int:
        case Short:
        case Character:
            output.Save("Number",saveAs+"T");
            break;
        case Date:
            output.Save("Date",saveAs+"T");
            break;
        case IndexedText:
        case Text:
            output.Save("Text",saveAs+"T");
            break;
        case Table:
            output.Save("Table",saveAs+"T");
            table->WriteStructureInternal(output,saveAs+"T");
            break;
    }
}

void DTTableColumn::Write(DTDataStorage &output,const std::string &saveAs) const
{
    output.Save(name,saveAs+"N");
    if (maskDefined)
        output.Save(mask,saveAs+"V_mask");
    switch (type) {
        case Empty:
            output.Save(0,saveAs+"V_T");
            break;
        case Double:
            output.Save("Number",saveAs+"V_T");
            output.Save(doubleVersion,saveAs+"V");
            break;
        case Float:
            output.Save("Number",saveAs+"V_T");
            output.Save(floatVersion,saveAs+"V");
            break;
        case Int:
            output.Save("Number",saveAs+"V_T");
            output.Save(intVersion,saveAs+"V");
            break;
        case Short:
            output.Save("Number",saveAs+"V_T");
            output.Save(shortVersion,saveAs+"V");
            break;
        case Character:
            output.Save("Number",saveAs+"V_T");
            output.Save(charVersion,saveAs+"V");
            break;
        case Date:
            output.Save("Date",saveAs+"V_T");
            output.Save(doubleVersion,saveAs+"V");
            break;
        case IndexedText:
            output.Save("Indexed",saveAs+"V_T");
            output.Save(charVersion,saveAs+"V_S");
            output.Save(intVersion,saveAs+"V");
            break;
        case Text:
            output.Save("UTF8",saveAs+"V_T");
            output.Save(stringVersion.Characters(),saveAs+"V");
            break;
        case Table:
            output.Save("Table",saveAs+"V_T");
            ::Write(output,saveAs+"V",*table);
            break;
    }

}

void DTTableColumn::WriteSingle(DTDataStorage &output,const std::string &saveAs) const
{
    if (maskDefined)
        output.Save(mask,saveAs+"_mask");
    switch (type) {
        case Empty:
            output.Save(DTDoubleArray(),saveAs);
            break;
        case Double:
            output.Save("Number",saveAs+"_T");
            output.Save(doubleVersion,saveAs);
            break;
        case Float:
            output.Save("Number",saveAs+"_T");
            output.Save(floatVersion,saveAs);
            break;
        case Int:
            output.Save("Number",saveAs+"_T");
            output.Save(intVersion,saveAs);
            break;
        case Short:
            output.Save("Number",saveAs+"_T");
            output.Save(shortVersion,saveAs);
            break;
        case Character:
            output.Save("Number",saveAs+"_T");
            output.Save(charVersion,saveAs);
            break;
        case Date:
            output.Save("Date",saveAs+"_T");
            output.Save(doubleVersion,saveAs);
            break;
        case IndexedText:
            output.Save("Indexed",saveAs+"_T");
            output.Save(charVersion,saveAs+"_S");
            output.Save(intVersion,saveAs);
            break;
        case Text:
            output.Save("UTF8",saveAs+"_T");
            output.Save(stringVersion.Characters(),saveAs);
            break;
        case Table:
            ::Write(output,saveAs,*table);
            break;
    }
    
}

#pragma mark Table Structure

// Check the structure of a table, so that it can be checked.
bool DTTableStructure::operator!=(const DTTableStructure &v) const
{
    int i,howMany = columns.Length();
    if (howMany!=v.columns.Length()) return true;
    
    for (i=0;i<howMany;i++) {
        if (columns(i)!=v.columns(i)) return true;
    }
    return false;
}

bool DTTableStructure::operator==(const DTTableStructure &v) const
{
    return !(this->operator!=(v));
}

#pragma mark Table

DTTable::DTTable(const DTList<DTTableColumn> &list)
{
    _columns = list;
    // Find the maximum row length
    int i,howMany = list.Length();
    int maxLen = 0;
    int thisLen;
    for (i=0;i<howMany;i++) {
        thisLen = list(i).NumberOfRows();
        if (thisLen>maxLen) maxLen = thisLen;
    }
    _numberOfRows = maxLen;
}

DTTable::ColumnType DTTable::Type(int col) const
{
    if (col<0 || col>=_columns.Length()) {
        DTErrorOutOfRange("DTTable", col,_columns.Length());
        return DTTable::OutOfBounds;
    }
    
    const DTTableColumn &column = _columns(col);
    DTTable::ColumnType toReturn = DTTable::Empty;
    
    switch (column.type) {
        case DTTableColumn::Empty:
            break;
        case DTTableColumn::Double:
        case DTTableColumn::Float:
        case DTTableColumn::Int:
        case DTTableColumn::Short:
        case DTTableColumn::Character:
            toReturn = DTTable::Numerical;
            break;
        case DTTableColumn::Date:
            toReturn = DTTable::Date;
            break;
        case DTTableColumn::IndexedText:
        case DTTableColumn::Text:
            toReturn = DTTable::Text;
            break;
        case DTTableColumn::Table:
            toReturn = DTTable::Table;
            break;
    }
    
    return toReturn;
}

DTTableColumn DTTable::Column(int col) const
{
    if (col<0 || col>=_columns.Length()) {
        DTErrorOutOfRange("DTTable", col,_columns.Length());
        return DTTableColumn();
    }

    return _columns(col);
}

DTTableColumn DTTable::operator()(const std::string &nm) const
{
    int col,howMany = _columns.Length();
    for (col=0;col<howMany;col++) {
        if (_columns(col).Name()==nm) {
            return _columns(col);
        }
    }
    DTErrorMessage("Table(name)","Column not found");
    return DTTableColumn(nm);
}

DTTableStructure DTTable::Structure(void) const
{
    int i,howMany = _columns.Length();
    DTMutableList<DTTableColumnStructure> columnStructure(howMany);
    for (i=0;i<howMany;i++) {
        columnStructure(i) = _columns(i).Structure();
    }
    DTTableStructure toReturn;
    toReturn.columns = columnStructure;
    return toReturn;
}

void DTTable::WriteStructureInternal(DTDataStorage &dataFile,const std::string &name) const
{
    int howManyColumns = NumberOfColumns();
    for (int i=0;i<howManyColumns;i++) {
        Column(i).WriteStructure(dataFile,name+"_"+DTInt2String(i+1));
    }
    
    dataFile.Save(howManyColumns,name+"_N");
}

void DTTable::WriteStructure(DTDataStorage &dataFile,const std::string &name) const
{
    std::string infoName = "SeqInfo_" + name;
    WriteStructureInternal(dataFile,infoName);
}

void DTTable::pinfoWithIndent(const std::string &indent) const
{
    int howMany = NumberOfColumns();
    for (int i=0;i<howMany;i++) {
        Column(i).pinfoWithIndent(indent);
    }
}

void DTTable::pinfo(void) const
{
#ifndef DG_NOSTDErrOut
    int howMany = NumberOfColumns();
    std::cerr << howMany << " columns and " << NumberOfRows() << " rows" << std::endl;
    pinfoWithIndent("  ");
#endif
}

void Read(const DTDataStorage &input,const std::string &name,DTTable &toReturn)
{
    if (input.Contains(name)==false) {
        toReturn = DTTable();
        return;
    }
    int howManyColumns = input.ReadInt(name);
    if (howManyColumns<=0) {
        toReturn = DTTable();
        return;
    }
    
    DTMutableList<DTTableColumn> columns(howManyColumns);
    
    int i;
    for (i=0;i<howManyColumns;i++) {
        columns(i) = DTTableColumn::Read(input,name+"_"+DTInt2String(i));
    }
 
    toReturn = DTTable(columns);
}
void Write(DTDataStorage &output,const std::string &name,const DTTable &theVar)
{
    int i,howMany = theVar.NumberOfColumns();
    for (i=0;i<howMany;i++) {
        theVar.Column(i).Write(output,name+"_"+DTInt2String(i));
    }
    
    output.Save(theVar.NumberOfRows(),name+"_R");
    output.Save(theVar.NumberOfColumns(),name);
}

void WriteOne(DTDataStorage &output,const std::string &name,const DTTable &toWrite)
{
    Write(output,name,toWrite);
    Write(output,"Seq_"+name,"Table");
    toWrite.WriteStructure(output,name);
    output.Flush();
}

