#ifndef _CONSTANTBUFFERCONTAINER_H
#define _CONSTANTBUFFERCONTAINER_H
#include "ConstantBuffer.h"

class ConstantBufferContainer{
public:
	ConstantBufferContainer();
	~ConstantBufferContainer();
	ConstantBuffer<TransformationBuffer>* cb_transform;
	ConstantBuffer<LightBuffer>* cb_light;
	ConstantBuffer<CameraBuffer>* cb_camera;
	ConstantBuffer<StreamoutParticleBuffer>* cb_streamParticle;
	ConstantBuffer<Particle>* cb_particle;
};
#endif