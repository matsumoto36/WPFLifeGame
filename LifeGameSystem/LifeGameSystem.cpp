#include "stdafx.h"
#include "LifeGameSystem.h"
#include <future>

// �v���W�F�N�g�̊֌W��A�����o�ɒu���Ȃ����ߖ������O��ԂŊǗ�����
namespace {
	std::thread MainThread;
}

namespace LifeGameSystem {

	void LifeGameSystem::SetCellLive(int clickPosX, int clickPosY, bool isLive) const {

		int cellPosX, cellPosY;
		// �}�E�X�̈ʒu�ɂ���Z�����擾
		_renderer->GetCellPosition(clickPosX, clickPosY, cellPosX, cellPosY);

		// �Z����ݒ�
		_cellManager->SetCellLive(cellPosX, cellPosY, isLive);
	}


	void LifeGameSystem::Start(int milliSec) {

		if (_isStarted) return;
		if (!_canStartLoop) return;

		_isStarted = true;

		// ���C�����[�v�����d�N���ł��Ȃ��悤�Ƀ��b�N����
		_canStartLoop = false;

		// �ʃX���b�h���s
		MainThread = thread(&LifeGameSystem::LoopAsync, this, milliSec, [this]() {
			// ���C�����[�v�̃��b�N����
			_canStartLoop = true;

			if (_onStoppedLoopOnce) {
				//���s�����瑦���ɏ�����
				_onStoppedLoopOnce();
				_onStoppedLoopOnce = nullptr;
			}
		

		});
		MainThread.detach();

	}

	void LifeGameSystem::LoopAsync(int milliSec, function<void()> callBack) {

		while (_isStarted) {

			// �~�߂�܂Ő�����X�V��������
			Next();
			this_thread::sleep_for(chrono::milliseconds(milliSec));
		}

		callBack();

	}


}

