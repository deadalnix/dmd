
// Compiler implementation of the D programming language
// All Rights Reserved
// written by Amaury SECHET
// License for redistribution is by either the Artistic License
// in artistic.txt, or the GNU General Public License in gnu.txt.
// See the included readme.txt for details.

// Handle attribute implementation

#include <stdio.h>
#include <assert.h>

#include "root.h"
#include "attribute.h"
#include "declaration.h"
#include "scope.h"
#include "module.h"
#include "mtype.h"
#include "identifier.h"

#define LOG     1

AttributeDeclaration::AttributeDeclaration(Loc loc, Loc endLoc, Identifier *id,
        TemplateParameters *parameters, Expression *constraint, Statement *body)
    : ScopeDsymbol(id)
{
#if LOG
    printf("AttributeDeclaration(this = %p, id = '%s')\n", this, id->toChars());
#endif
    
    Identifier* astTypeId = Lexer::uniqueId("__AstType");
    parameters->push(new TemplateTypeParameter(loc, astTypeId, NULL, NULL));
    
    Type* astType = new TypeIdentifier(loc, astTypeId);
    Parameters* ps = new Parameters();
    ps->push(new Parameter(0, astType, Lexer::idPool("ast"), NULL));
    
    FuncDeclaration* f = new FuncDeclaration(loc, endLoc, id, STCundefined, new TypeFunction(ps, astType, 0, LINKd, STCundefined));
    f->fbody = body;
    
    Dsymbols *decldefs = new Dsymbols();
    decldefs->push(f);
    
    tpl = new TemplateDeclaration(loc, id, parameters, constraint, decldefs, 0);
}

void AttributeDeclaration::semantic(Scope *sc) {
#if LOG
    printf("AttributeDeclaration::semantic(this = %p, id = '%s')\n", this, ident->toChars());
    printf("sc->stc = %llx\n", sc->stc);
    printf("sc->module = %s\n", sc->module->toChars());
#endif

	tpl->semantic(sc);
}

AttributeInstance::AttributeInstance(Loc loc, Identifier *attr_id, Dsymbols* decl, Objects* arglist)
    : AttribDeclaration(decl)
{
#if LOG
    printf("AttributeInstance(this = %p, ident = '%s')\n", this, attr_id ? attr_id->toChars() : "null");
#endif

     this->loc = loc;
     this->attr_id = attr_id;
     this->arglist = arglist;
}

void AttributeInstance::semantic(Scope *sc) {
#if LOG
    printf("AttributeInstance::semantic(this = %p)\n", this);
    printf("sc->stc = %llx\n", sc->stc);
    printf("sc->module = %s\n", sc->module->toChars());
#endif

	Dsymbol* dsym = sc->search(loc, attr_id, NULL);
	if(dsym == NULL) {
	    error("impossible to find attribute @%s", attr_id->toChars());
	    return;
	}
	
	AttributeDeclaration* attrdecl = dsym->isAttributeDeclaration();
	if(attrdecl == NULL) {
	    error("%s is not an attribute", attr_id->toChars());
	    return;
	}
	
	Expressions* args = new Expressions();
	args->push(new IntegerExp(loc, 42, Type::tint32));
	
	Expressions* args2 = new Expressions();
	args2->push(new NewExp(loc, NULL, new Expressions(), Type::tint32, args));
	
	arglist->push(Type::tint32);
	TemplateInstance *i = new TemplateInstance(loc, attrdecl->tpl, arglist);
	i->semantic(sc);
	arglist->pop();
	
	AttribDeclaration::semantic(sc);
}

