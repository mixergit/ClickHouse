#pragma once

#include <Parsers/IAST.h>
#include <Core/Names.h>


namespace DB
{

struct ASTTablesInSelectQueryElement;


/** SELECT query
  */
class ASTSelectQuery : public IAST
{
public:
    enum class Expression : UInt8
    {
        WITH,
        SELECT,
        TABLES,
        PREWHERE,
        WHERE,
        GROUP_BY,
        HAVING,
        ORDER_BY,
        LIMIT_BY_VALUE,
        LIMIT_BY,
        LIMIT_OFFSET,
        LIMIT_LENGTH,
        SETTINGS
    };

    /** Get the text that identifies this element. */
    String getID(char) const override { return "SelectQuery"; }

    ASTPtr clone() const override;

    bool distinct = false;
    bool group_by_with_totals = false;
    bool group_by_with_rollup = false;
    bool group_by_with_cube = false;

    ASTPtr & refSelect()    { return getExpression(Expression::SELECT); }
    ASTPtr & refTables()    { return getExpression(Expression::TABLES); }
    ASTPtr & refPrewhere()  { return getExpression(Expression::PREWHERE); }
    ASTPtr & refWhere()     { return getExpression(Expression::WHERE); }
    ASTPtr & refHaving()    { return getExpression(Expression::HAVING); }

    const ASTPtr with()           const { return getExpression(Expression::WITH); }
    const ASTPtr select()         const { return getExpression(Expression::SELECT); }
    const ASTPtr tables()         const { return getExpression(Expression::TABLES); }
    const ASTPtr prewhere()       const { return getExpression(Expression::PREWHERE); }
    const ASTPtr where()          const { return getExpression(Expression::WHERE); }
    const ASTPtr groupBy()        const { return getExpression(Expression::GROUP_BY); }
    const ASTPtr having()         const { return getExpression(Expression::HAVING); }
    const ASTPtr orderBy()        const { return getExpression(Expression::ORDER_BY); }
    const ASTPtr limitByValue()   const { return getExpression(Expression::LIMIT_BY_VALUE); }
    const ASTPtr limitBy()        const { return getExpression(Expression::LIMIT_BY); }
    const ASTPtr limitOffset()    const { return getExpression(Expression::LIMIT_OFFSET); }
    const ASTPtr limitLength()    const { return getExpression(Expression::LIMIT_LENGTH); }
    const ASTPtr settings()       const { return getExpression(Expression::SETTINGS); }

    /// Set/Reset/Remove expression.
    void setExpression(Expression expr, ASTPtr && ast);

    ASTPtr getExpression(Expression expr, bool clone = false) const
    {
        auto it = positions.find(expr);
        if (it != positions.end())
            return clone ? children[it->second]->clone() : children[it->second];
        return {};
    }

    /// Compatibility with old parser of tables list. TODO remove
    ASTPtr sample_size() const;
    ASTPtr sample_offset() const;
    ASTPtr array_join_expression_list(bool & is_left) const;
    ASTPtr array_join_expression_list() const;
    const ASTTablesInSelectQueryElement * join() const;
    bool final() const;
    void replaceDatabaseAndTable(const String & database_name, const String & table_name);
    void addTableFunction(ASTPtr & table_function_ptr);

protected:
    void formatImpl(const FormatSettings & settings, FormatState & state, FormatStateStacked frame) const override;

private:
    std::unordered_map<Expression, size_t> positions;

    ASTPtr & getExpression(Expression expr);
};

}
