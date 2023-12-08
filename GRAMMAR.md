# The grammar

```c++
<program>                       ::= <statement> '\n' (<statement> '\n')* 'run' '\n'
<statement>                     ::= <variable_declaration> | <assignment> | <print_statement> | <conditional_statement>

<variable_declaration>          ::= 'var' <data_type> <identifier> '=' <expression>
<data_type>                     ::= 'int' | 'float'
<identifier>                    ::= (<letter> | '_') <identifier_characters>*
<identifier_characters>         ::= <letter> | <digit> | '_'
<assignment>                    ::= <identifier> '=' <expression>

<expression>                    ::= <term> (('+' | '-') <term>)*
<term>                          ::= <factor> (('*' | '/') <factor>)*
<factor>                        ::= <literal> | <identifier> | '(' <expression> ')'

<literal>                       ::= <integer_literal> | <float_literal>
<integer_literal>               ::= <digit>+
<float_literal>                 ::= <digit>+ '.' <digit>+

<comparison>                    ::= <expression> <comparator> <expression>
<comparator>                    ::= '<' | '>' | '==' | '!='

<conditional_statement>         ::= <while_statement> 
                                | <if_statement>
<while_statement>               ::= 'while' '(' <comparison> ')' ':' '\n' <indented_statement_list>
<if_statement>                  ::= 'if' '(' <comparison> ')' ':' '\n' <indented_statement_list> <elif_statements> <else_statement>?
<elif_statements>               ::= ('elif' '(' <comparison> ')' ':' '\n' <indented_statement_list>)*
<else_statement>                ::= 'else' ':' '\n' <indented_statement_list>

<indented_statement_list>       ::= <tab> <indented_statement>+
<indented_statement>            ::= <statement> '\n'

<print_statement>               ::= 'print' (<expression> | <identifier> | <string_literal>)
<string_literal>                ::= '"' <any_character>* '"'
<any_character>                 ::= <any printable ASCII character>

<tab>                           ::= '\t' 

<letter>                        ::= 'a'-'z' | 'A'-'Z'
<digit>                         ::= '0'-'9'
```