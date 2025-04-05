#include "Token.h"

#include "Context.h"

void nikiInsertReferencesInToken(NikiContext* pCtx, NikiToken* pToken) {
	size_t offset = 0;
	for (NIKI_REFERENCES_SIZE_TYPE i = 0; i < pToken->references.size; ++i) {
		if (pCtx->consoleVariables.count(reference.second) != 0) { // console variable
			token.value = token.value.insert(offset+reference.first, pCtx->pConsoleVariables[reference.second]);
			offset += pCtx->consoleVariables[reference.second].size();

		} else if (pCtx->programVariables.count(reference.second) != 0) { // program variable
			ProgramVariable& var = pCtx->programVariables[reference.second];
			sds value = var.get(ctx, &var);

			token.value = token.value.insert(offset+reference.first, value);
			offset += value.size();
		
		} else {
			token.value = token.value.insert(offset+reference.first, formatString("{}{}{}{}", NIKI_REFERENCE, NIKI_REFERENCE_OPEN, reference.second, NIKI_REFERENCE_CLOSE));
			offset += reference.second.size()+3;
		}
	}
}