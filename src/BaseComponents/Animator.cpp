#include"Animator.h"
#include "../Utilities/AssimpMath.h"
#include <../../glm/glm/gtx/matrix_decompose.hpp>
#include <../../glm/glm/gtx/matrix_interpolation.hpp>

void AnimationState::start()
{
    _currentTime = 0;
}

void AnimationState::update(float deltaTime)
{
    std::map<std::string, glm::mat4> transforms;

    getClipTransforms(_currentTime, transforms);
    _currentTime += deltaTime;
    
    for(auto i = 0u; i < _container->attachedMeshCount(); i++)
    {
        auto& mesh = (*_container)[i]; 
        std::vector<glm::mat4> meshTransforms(mesh.bonesCount());

        for(auto& element : transforms){
            uint boneId;

            if(mesh.tryFindBoneID(element.first, boneId))
            {   
                meshTransforms[boneId] = element.second;
            }
        }

		if(_currentTime < _transitDuration)
        	mesh.setBoneTransforms(meshTransforms, _currentTime / _transitDuration);
		else
			mesh.setBoneTransforms(meshTransforms);
    }
	
	std::vector<std::string> events;

	getEvents(_currentTime, events);

	for(auto& event : events) {
		if(_listenerPool.contains(event))
		{
			auto &vec = _listenerPool[event];

			for(auto &elem : vec){
				elem->handle();
			}
		}

	}
}

void AnimationState::addListener(std::string eventName, Observer* listener)
{
	if(_listenerPool.contains(eventName) == false)
		_listenerPool[eventName] = std::vector<Observer*>();

	_listenerPool[eventName].push_back(listener);
}

void SingleAnimationState::getEvents(float currentTime, std::vector<std::string> &eventNames){
	float timeTicks = _clip->scoreTimeInTicks(currentTime);
	_clip->processEvents(timeTicks, _prevTimeTicks, eventNames);
	_prevTimeTicks = timeTicks;
}

void BlendTree::getEvents(float currentTime, std::vector<std::string> &eventNames){
	float timeTicks = _clips[0].Clip->scoreTimeInTicks(currentTime);
	_clips[0].Clip->processEvents(timeTicks, _prevTimeTicks, eventNames);
	_prevTimeTicks = timeTicks;
}

void SingleAnimationState::getClipTransforms(float currentTime, std::map<std::string, glm::mat4>& transforms)
{
    return _clip->GetBoneTransforms(_clip->scoreTimeInTicks(currentTime), transforms);
}

void BlendTree::getClipTransforms(float currentTime, std::map<std::string, glm::mat4>& transforms)
{
	_clips[0].Clip->scoreTimeInTicks(currentTime);

	std::map<std::string, glm::mat4> firstTransforms;
	std::map<std::string, glm::mat4> secondTransforms;
	float actualFactor;

	if(_blendFactor > 1)
		_blendFactor = 1;
		
	for(int i = 0; i < _clips.size() - 1; i++)
	{
		if(_clips[i+1].Value >= _blendFactor)
		{
			_clips[i].Clip->GetBoneTransforms(_clips[i].Clip->scoreTimeInTicks(currentTime), firstTransforms);
			_clips[i+1].Clip->GetBoneTransforms(_clips[i+1].Clip->scoreTimeInTicks(currentTime), secondTransforms);
			actualFactor = (_blendFactor - _clips[i].Value) / (_clips[i+1].Value - _clips[i].Value);
			break;
		}
	}
	for(auto& firstElement : firstTransforms)
	{
		glm::vec3 firstScale;
		glm::quat firstRotation;
		glm::vec3 firstTranslation;
		glm::vec3 secondScale;
		glm::quat secondRotation;
		glm::vec3 secondTranslation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(firstElement.second, firstScale, firstRotation, firstTranslation, skew, perspective);
		glm::decompose(secondTransforms[firstElement.first], secondScale, secondRotation, secondTranslation, skew, perspective);

		glm::mat4 translationM = glm::translate(glm::mat4(1.f), firstTranslation + actualFactor*(secondTranslation - firstTranslation));
		glm::mat4 scaleM = glm::scale(glm::mat4(1.f), firstScale + actualFactor*(secondScale - firstScale));
		glm::mat4 rotationM = glm::interpolate(glm::mat4_cast(firstRotation), glm::mat4_cast(secondRotation), actualFactor);
		transforms[firstElement.first] = translationM * rotationM * scaleM;
	}
}

unsigned int AnimationClip::FindScaling(float _animationTime, const aiNodeAnim* _nodeAnim)
		{
			assert(_nodeAnim->mNumScalingKeys > 0);
			for (unsigned int i = 0; i < _nodeAnim->mNumScalingKeys - 1; i++) {
				if (_animationTime < (float)_nodeAnim->mScalingKeys[i + 1].mTime) {
					return i;
				}
			}

			// It should never reach here.
			assert(0);
			return 0;
		}

		unsigned int AnimationClip::FindRotation(float _animationTime, const aiNodeAnim* _nodeAnim)
		{
			assert(_nodeAnim->mNumRotationKeys > 0);
			for (unsigned int i = 0; i < _nodeAnim->mNumRotationKeys - 1; i++) {
				if (_animationTime < (float)_nodeAnim->mRotationKeys[i + 1].mTime) {
					return i;
				}
			}

			// It should never reach here.
			assert(0);
			return 0;
		}

		unsigned int AnimationClip::FindPosition(float _animationTime, const aiNodeAnim* _nodeAnim)
		{
			assert(_nodeAnim->mNumPositionKeys > 0);
			for (unsigned int i = 0; i < _nodeAnim->mNumPositionKeys - 1; i++) {
				if (_animationTime < (float)_nodeAnim->mPositionKeys[i + 1].mTime) {
					return i;
				}
			}

			// It should never reach here.
			assert(0);
			return 0;
		}

		void AnimationClip::CalcInterpolatedPosition(aiVector3D& _out, float _animationTime, const aiNodeAnim* _nodeAnim)
		{
			if (_nodeAnim->mNumPositionKeys == 1) {
				// There is only one Position.
				_out = _nodeAnim->mPositionKeys[0].mValue;
				return;
			}

			unsigned int position_index = FindPosition(_animationTime, _nodeAnim);
			unsigned int next_pos_index = position_index + 1;
			assert(next_pos_index < _nodeAnim->mNumPositionKeys);

			// The Difference between two key frames.
			float delta_time = (float)(_nodeAnim->mPositionKeys[next_pos_index].mTime - _nodeAnim->mPositionKeys[position_index].mTime);

			// The Factor by which the current frame has transitioned into the next frame.
			float factor = (_animationTime - (float)_nodeAnim->mPositionKeys[position_index].mTime) / delta_time;

			assert(factor >= 0.0f && factor <= 1.0f);

			const auto start = _nodeAnim->mPositionKeys[position_index].mValue;
			const auto end = _nodeAnim->mPositionKeys[next_pos_index].mValue;

			_out = start + factor * (end - start);

		}

		void AnimationClip::CalcInterpolatedRotation(aiQuaternion& _out, float _animationTime, const aiNodeAnim* _nodeAnim)
		{

			if (_nodeAnim->mNumRotationKeys == 1) {
				// There is only one Position.
				_out = _nodeAnim->mRotationKeys[0].mValue;
				return;
			}

			unsigned int rotation_index = FindRotation(_animationTime, _nodeAnim);
			unsigned int next_rot_index = rotation_index + 1;
			assert(next_rot_index < _nodeAnim->mNumRotationKeys);

			// The Difference between two key frames.
			float delta_time = (float)(_nodeAnim->mRotationKeys[next_rot_index].mTime - _nodeAnim->mRotationKeys[rotation_index].mTime);

			// The Factor by which the current frame has transitioned into the next frame.
			float factor = (_animationTime - (float)_nodeAnim->mRotationKeys[rotation_index].mTime) / delta_time;

			assert(factor >= 0.0f && factor <= 1.0f);

			const auto start = _nodeAnim->mRotationKeys[rotation_index].mValue;
			const auto end = _nodeAnim->mRotationKeys[next_rot_index].mValue;

			aiQuaternion::Interpolate(_out, start, end, factor);

			_out.Normalize();

		}

		void AnimationClip::CalcInterpolatedScaling(aiVector3D& _out, float _animationTime, const aiNodeAnim* _nodeAnim)
		{

			if (_nodeAnim->mNumScalingKeys == 1) {
				_out = _nodeAnim->mScalingKeys[0].mValue;
				return;
			}

			auto scaling_index = FindScaling(_animationTime, _nodeAnim);
			auto nex_sca_index = scaling_index + 1;

			assert(nex_sca_index < _nodeAnim->mNumScalingKeys);

			auto delta_time = (float)(_nodeAnim->mScalingKeys[nex_sca_index].mTime - _nodeAnim->mScalingKeys[scaling_index].mTime);

			auto factor = (_animationTime - (float)_nodeAnim->mScalingKeys[scaling_index].mTime) / delta_time;

			assert(factor >= 0.0f && factor <= 1.0f);

			auto start = _nodeAnim->mScalingKeys[scaling_index].mValue;
			auto end = _nodeAnim->mScalingKeys[nex_sca_index].mValue;

			_out = start + factor * (end - start);

		}

void AnimationClip::ReadNodeHierarchy(float AnimationTimeTicks, const aiNode* pNode, const glm::mat4& ParentTransform, std::map<std::string, glm::mat4>& Transforms)
{
    std::string NodeName(pNode->mName.data);

    glm::mat4 NodeTransformation;
    AssimpMath::convert_aimatrix_to_glm(NodeTransformation, pNode->mTransformation);
    const aiNodeAnim* pNodeAnim = FindNodeAnim(NodeName);
    
    if (pNodeAnim) {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, AnimationTimeTicks, pNodeAnim);
        glm::mat4 ScalingM(1.0f);
        ScalingM = glm::scale(ScalingM, glm::vec3(Scaling.x, Scaling.y, Scaling.z));

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, AnimationTimeTicks, pNodeAnim);
        glm::quat rotation = AssimpMath::getGLMQuat(RotationQ);
        glm::mat4 RotationM = glm::mat4_cast(rotation);

        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        CalcInterpolatedPosition(Translation, AnimationTimeTicks, pNodeAnim);
        glm::mat4 TranslationM(1.0f);
        TranslationM = glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));

        // Combine the above transformations
        NodeTransformation = TranslationM * RotationM * ScalingM;
    }

    glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;


    if(_boneOffsets.find(NodeName) != _boneOffsets.end())
        Transforms[NodeName] = _globalInverse * GlobalTransformation * _boneOffsets[NodeName];



    for (uint i = 0 ; i < pNode->mNumChildren ; i++) {
        ReadNodeHierarchy(AnimationTimeTicks, pNode->mChildren[i], GlobalTransformation, Transforms);
    }
}

float AnimationClip::getDuration()
{
	float TicksPerSecond = (float)(_animation->mTicksPerSecond != 0 ? _animation->mTicksPerSecond : 25.0f) * _speed;
	return _animation->mDuration / TicksPerSecond;
}

void AnimationClip::changeSpeed(float newSpeed)
{
	if(newSpeed <= 0)
		throw std::logic_error("Trying to set negative animation speed");

	_speed = newSpeed;
}

void AnimationClip::GetBoneTransforms(float timeTicks, std::map<std::string, glm::mat4>& Transforms)
{
    glm::mat4 Identity(1.0f);

    ReadNodeHierarchy(timeTicks, _rootNode, Identity, Transforms);
}

void AnimationClip::processEvents(float timeTicks, float prevTimeTicks, std::vector<std::string> &eventNames){
	for(int i = 0; i < _events.size(); i++){
		if(_events[i].timeInTicks > prevTimeTicks && _events[i].timeInTicks < timeTicks)
			eventNames.push_back(_events[i].name);
	}
}

void AnimationClip::addEvent(std::string name, float timeInTicks){
	EventPair newPair;
	newPair.name = name;
	newPair.timeInTicks = timeInTicks;
	_events.push_back(newPair);
	std::sort(_events.begin(), _events.end());
}

const aiNodeAnim* AnimationClip::FindNodeAnim(const std::string& NodeName)
{
    for (uint j = 0 ; j < _animation->mNumChannels; j++) {
    const aiNodeAnim* pNodeAnim = _animation->mChannels[j];
    }

    for (uint i = 0 ; i < _animation->mNumChannels; i++) {
        const aiNodeAnim* pNodeAnim = _animation->mChannels[i];
        
        if (std::string(_animation->mChannels[i]->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }
    return NULL;
}

float AnimationClip::scoreTimeInTicks(float currentTime)
{
    float TicksPerSecond = (float)(_animation->mTicksPerSecond != 0 ? _animation->mTicksPerSecond : 25.0f) * _speed;
    float TimeInTicks = currentTime * TicksPerSecond;
    return fmod(TimeInTicks, (float)_animation->mDuration);
}