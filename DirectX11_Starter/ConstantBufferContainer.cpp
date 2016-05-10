#include "ConstantBufferContainer.h"



ConstantBufferContainer::ConstantBufferContainer()
{
	TransformationBuffer transform;
	cb_transform = new ConstantBuffer<TransformationBuffer>(transform);
	LightBuffer light;
	cb_light = new ConstantBuffer<LightBuffer>(light);
	CameraBuffer camera;
	cb_camera = new ConstantBuffer<CameraBuffer>(camera);
	StreamoutParticleBuffer stream;
	cb_streamParticle = new ConstantBuffer<StreamoutParticleBuffer>(stream);
	Particle particle;
	cb_particle = new ConstantBuffer<Particle>(particle);
}

ConstantBufferContainer::~ConstantBufferContainer()
{

}