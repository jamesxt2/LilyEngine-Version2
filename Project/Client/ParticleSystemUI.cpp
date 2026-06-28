#include "pch.h"
#include "ParticleSystemUI.h"

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("ParticleSystem", "##ParticleSystemUI", COMPONENT_TYPE::PARTICLESYSTEM)
{
	SetChildSize(ImVec2(0.f, 300.f));
}

ParticleSystemUI::~ParticleSystemUI()
{
}

void ParticleSystemUI::Render_Tick()
{
	Render_Title();
}
