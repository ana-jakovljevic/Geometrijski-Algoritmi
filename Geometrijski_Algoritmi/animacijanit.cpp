#include "animacijanit.h"
#include "algoritambaza.h"

AnimacijaNit::AnimacijaNit(AlgoritamBaza *pAlgoritam)
    :QThread{}, _pAlgoritam(pAlgoritam)
{}

void AnimacijaNit::run()
{
    if (_pAlgoritam)
        _pAlgoritam->pokreniAlgoritam();
}
