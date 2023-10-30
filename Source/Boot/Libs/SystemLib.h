#pragma once

void Reboot()
{
    RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, 0);
}

void Restart()
{
    RuntimeServices->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, 0);
}

void Shutdown()
{
    RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, 0);
}
