#include "Transform.h"


void Transform::Identity() {
	vSca = { 1,1,1 };
	XMStoreFloat4x4(&mSca, XMMatrixIdentity());

	vDir = { 0,0,0 };
	



}