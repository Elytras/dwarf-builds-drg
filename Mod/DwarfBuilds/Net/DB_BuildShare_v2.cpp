// DB_BuildShare_v2: what a player's own mod sends the host.
#include "../DwarfBuilds.h"

namespace Net {

// Server, from the owning player only (the engine routes a Server RPC through the owner's connection). A code that
// is not a DB1 code, or longer than any build, is ignored; the readers parse it like a pasted code.
void DB_BuildShare_v2::Server_SetCode(const FString& NewCode) {
    if (UKismetStringLibrary::Len(NewCode) > 600 ||
        !UKismetStringLibrary::StartsWith(NewCode, "DB1:", ESearchCase::CaseSensitive))
        return;
    Code = NewCode;
    PerksKnown = true;
    FromPlayer = true;
}

}   // namespace Net
