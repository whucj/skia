/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
 
#ifndef SKSL_ASTEXTENSION
#define SKSL_ASTEXTENSION

#include "SkSLASTDeclaration.h"

namespace SkSL {

/** 
 * An extension declaration. 
 */
struct ASTExtension : public ASTDeclaration {
    ASTExtension(Position position, SkString name)
    : INHERITED(position, kExtension_Kind)
    , fName(std::move(name)) {}

    SkString description() const override {
        return "#extension " + fName + " : enable";
    }

    const SkString fName;

    typedef ASTDeclaration INHERITED;
};

} // namespace

#endif
