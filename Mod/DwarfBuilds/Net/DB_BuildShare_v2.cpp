// DB_BuildShare_v2: what a player's own mod sends the host, and telling this machine's DB_Data about the link.
#include "../DwarfBuilds.h"

namespace Net {

namespace {
const FString CodeChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:-.";   // every character a build code has
}

// Server, from the owning player only (the engine routes a Server RPC through the owner's connection). Ignored: a
// code that does not start with DB1:, is longer than any build, or holds a character no build code has, so a player
// cannot put arbitrary text into what others copy with COPY CODE. The readers parse the rest like a pasted code. The
// change is pushed at once (the link replicates rarely), and the host's DB_Data is told.
void DB_BuildShare_v2::Server_SetCode(const FString& NewCode) {
    if (UKismetStringLibrary::Len(NewCode) > 600 ||
        !UKismetStringLibrary::StartsWith(NewCode, "DB1:", ESearchCase::CaseSensitive))
        return;
    TArray<FString> Chars = UKismetStringLibrary::GetCharacterArrayFromString(NewCode);
    for (const FString& Char : Chars)
        if (UKismetStringLibrary::FindSubstring(CodeChars, Char, true, false, -1) < 0) return;
    PerksKnown = true;
    FromPlayer = true;
    Code = NewCode;
    ForceNetUpdate();
    Tell();
}

void DB_BuildShare_v2::ReceiveBeginPlay() {
    Tell();
}

void DB_BuildShare_v2::OnRep_Player() {
    Tell();
}

// No DB_Data yet: it finds the link itself when it spawns (ShareScan).
void DB_BuildShare_v2::Tell() {
    DB_Data* Data = (DB_Data*)UGameplayStatics::GetActorOfClass(this, DB_Data::StaticClass());
    if (UKismetSystemLibrary::IsValid(Data)) Data->LinkChanged(this);
}

}   // namespace Net
