// Part of DTSource. Copyright 2017. David Adalsteinsson.
// see http://www.visualdatatools.com/DTSource/license.html for more information.

#ifndef DTTable_H
#define DTTable_H

#include "DTIntArray.h"
#include "DTDoubleArray.h"
#include "DTFloatArray.h"
#include "DTIntArray.h"
#include "DTShortIntArray.h"
#include "DTCharArray.h"
#include "DTStringList.h"
#include "DTList.h"

class DTDataStorage;
class DTTable;
struct DTTableStructure;
struct DTRange;


// A column can be a number, text or date, and you can also have sub-tables.
struct DTTableColumnStructure
{
    std::string name;
    std::string type; // Number, Text, Date, Table
    DTPointer<DTTableStructure> tableStructure;
    
    bool operator!=(const DTTableColumnStructure &) const;
    bool operator==(const DTTableColumnStructure &) const;
};

class DTTableColumn
{
public:
    enum StorageType {Empty,Double, Float, Int, Short, Character, Date, IndexedText, Text, Table};
    
    DTTableColumn() : type(DTTableColumn::Empty), maskDefined(false) {}
    explicit DTTableColumn(const std::string &nm) : name(nm), type(DTTableColumn::Empty), maskDefined(false) {}
    
    static DTTableColumn TextColumn(const std::string &,const DTIntArray &,const DTCharArray &);
    static DTTableColumn TextColumn(const std::string &,const DTStringList &);

    static DTTableColumn NumberColumn(const std::string &,const DTIntArray &,const DTCharArray &);
    static DTTableColumn NumberColumn(const std::string &,const DTIntArray &);
    static DTTableColumn NumberColumn(const std::string &,const DTDoubleArray &,const DTCharArray &);
    static DTTableColumn NumberColumn(const std::string &,const DTDoubleArray &);
    static DTTableColumn NumberColumn(const std::string &,const DTShortIntArray &,const DTCharArray &);
    static DTTableColumn NumberColumn(const std::string &,const DTShortIntArray &);
    static DTTableColumn NumberColumn(const std::string &,const DTCharArray &,const DTCharArray &);
    static DTTableColumn NumberColumn(const std::string &,const DTCharArray &);
    
    static DTTableColumn DateColumn(const std::string &,const DTDoubleArray &,const DTCharArray &);
    static DTTableColumn DateColumn(const std::string &,const DTDoubleArray &);
    
    static DTTableColumn TableColumn(const std::string &,const DTTable &);

    bool IsEmpty(void) const {return (type==Empty);}
    bool IsNumberColumn(void) const {return (type==Double || type==Float || type==Int || type==Short || type==Character);}
    bool IsTextColumn(void) const {return (type==IndexedText || type==Text);}
    bool IsDateColumn(void) const {return (type==Date);}
    bool IsTable(void) const {return (type==Table);}
    
    // Get data
    int NumberOfRows(void) const;
    DTTableColumnStructure Structure(void) const;
    DTDoubleArray NumberArray(void) const;
    
    DTTableColumn ExtractRows(const DTRange &) const;
    DTTableColumn ChangeName(const std::string &) const;
    std::string Name(void) const {return name;}

    void pinfoWithIndent(const std::string &) const;
    
    static DTTableColumn Read(const DTDataStorage &output,const std::string &name);
    void Write(DTDataStorage &output,const std::string &name) const;
    void WriteSingle(DTDataStorage &output,const std::string &name) const;
    void WriteStructure(DTDataStorage &dataFile,const std::string &name) const;
    
private:
    std::string name;
    StorageType type;
    bool maskDefined;

    DTDoubleArray doubleVersion;
    DTFloatArray floatVersion;
    DTIntArray intVersion;
    DTShortIntArray shortVersion;
    DTCharArray charVersion;
    
    DTStringList stringVersion;
    DTCharArray mask;
    
    DTPointer<DTTable> table;
    
    friend class DTTable;
};

// Check the structure of a table, so that it can be checked.
struct DTTableStructure
{
    DTList<DTTableColumnStructure> columns;

    bool operator!=(const DTTableStructure &) const;
    bool operator==(const DTTableStructure &) const;
};

class DTTable
{
public:
    DTTable() : _numberOfRows(0) {}
    DTTable(const DTList<DTTableColumn> &);
    
    enum ColumnType {OutOfBounds,Empty,Numerical, Date, Text, Table};
    
    bool IsEmpty(void) const {return (_columns.Length()==0);}
    int NumberOfColumns(void) const {return _columns.Length();}
    int NumberOfRows(void) const {return _numberOfRows;}
    
    ColumnType Type(int col) const;
    DTTableColumn Column(int col) const;
    DTTableColumn operator()(const std::string &) const;
    DTTableStructure Structure(void) const;
    
    void pinfo(void) const;
    
    void WriteStructure(DTDataStorage &,const std::string &) const;
    
private:
    void pinfoWithIndent(const std::string &) const;
    void WriteStructureInternal(DTDataStorage &dataFile,const std::string &name) const;
    
    DTList<DTTableColumn> _columns;
    int _numberOfRows;

    friend class DTTableColumn;
};

// Reading and writing
extern void Read(const DTDataStorage &input,const std::string &name,DTTable &toReturn);
extern void Write(DTDataStorage &output,const std::string &name,const DTTable &theVar);
extern void WriteOne(DTDataStorage &output,const std::string &name,const DTTable &toWrite); // One time value, self documenting.

#endif
