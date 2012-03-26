
// Compiler implementation of the D programming language
// All Rights Reserved
// written by Amaury SECHET
// License for redistribution is by either the Artistic License
// in artistic.txt, or the GNU General Public License in gnu.txt.
// See the included readme.txt for details.

#ifndef DMD_ATTRIBUTE_H
#define DMD_ATTRIBUTE_H

#ifdef __DMC__
#pragma once
#endif /* __DMC__ */

#include "root.h"
#include "arraytypes.h"
#include "dsymbol.h"
#include "template.h"

struct AttributeDeclaration : ScopeDsymbol
{
    TemplateDeclaration* tpl;
    
    AttributeDeclaration(Loc loc, Loc endLoc, Identifier *id, TemplateParameters *parameters,
        Expression *constraint, Statement *body);
    void semantic(Scope *sc);
};

#endif /* DMD_ATTRIBUTE_H */

