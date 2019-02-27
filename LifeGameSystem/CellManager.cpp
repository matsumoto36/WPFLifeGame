#include "stdafx.h"
#include "CellManager.h"

namespace LifeGameSystem {

	CellManager::CellManager(int width, int height) :
		BitSize(sizeof(uint_fast64_t) * 8),							// 型情報から計算
		_dataSizeWidth(ceil(static_cast<float>(width) / BitSize)),	// 横はBitSize分を1つとして保持
		_dataSizeHeight(height) {									// 縦は変わらないのでそのまま

		//配列を初期化
		Clear();
	}

	bool CellManager::SetCellLive(int x, int y, bool enable) {

		//境界チェック
		if (!CheckBorder(x, y)) return false;

		int offset = GetByteOffset(x, y);
		uint_fast64_t mask = GetByteMask(x);

		if (enable) _cellData[offset] |= mask;
		else        _cellData[offset] &= ~mask;

		return true;
	}

	void CellManager::GenerationUpdate() {

		// のちにスワップするので、余分に確保しておく
		vector<uint_fast64_t> buff((_dataSizeWidth + 2) * (_dataSizeHeight + 2), 0ULL);

		// 余分に確保した分ずらす
		for (int y = 1; y <= _dataSizeHeight; y++) {
			for (int x = 1; x <= _dataSizeWidth; x++) {

				// 配列上の位置を計算
				int offset = x + y * (_dataSizeWidth + 2);

				//1ブロック更新
				buff[offset] = NextGeneration(offset);
			}
		}

		//更新を反映
		_cellData.swap(buff);
	}

	uint_fast64_t CellManager::NextGeneration(int offset) {
	
		// 高速化のため、ビットボードで行う //

		// a~hを加算して、ビットごとの1の数を求める
		// ライフゲームでは、ビットごとの数が2個のときと3個のときのものだけでよい

		// 上部
		auto upOffset = offset - (2 + _dataSizeWidth);
		auto up = _cellData[upOffset];

		// 中央
		auto data = _cellData[offset];

		// 下部
		auto downOffset = offset + (2 + _dataSizeWidth);
		auto down = _cellData[downOffset];

		// 最上位ビットを保存しておく
		const auto MSB = 0x8000000000000000ULL;

		// 八近傍を求める
		auto a = (up >> 1) | (!(_cellData[upOffset - 1] & 0x01) ? 0 : MSB);
		auto b = up;
		auto c = (up << 1) | (!(_cellData[upOffset + 1] & 0x80) ? 0 : 1);
		auto d = (data >> 1) | (!(_cellData[offset - 1] & 0x01) ? 0 : MSB);
		auto e = (data << 1) | (!(_cellData[offset + 1] & 0x80) ? 0 : 1);
		auto f = (down >> 1) | (!(_cellData[downOffset - 1] & 0x01) ? 0 : MSB);
		auto g = down;
		auto h = (down << 1) | (!(_cellData[downOffset - 1] & 0x80) ? 0 : 1);

		// 合計値を早く求めるために、ビット演算で求める
		auto xab = a & b;	//	a + b の上位ビット
		a ^= b;
		auto xcd = c & d;
		c ^= d;
		auto xef = e & f;
		e ^= f;
		auto xgh = g & h;
		g ^= h;

		// (xab a) + (xcd c) → (c b a)		論理演算回数：５
		d = a & c;
		a ^= c;
		c = xab & xcd;		//	b2 が１になるのは (1 0) + (1 0) の時のみ
		b = xab ^ xcd ^ d;

		// (xef e) + (xgh g) → (g f e)		論理演算回数：５
		h = e & g;
		e ^= g;
		g = xef & xgh;		//	b2 が１になるのは (1 0) + (1 0) の時のみ
		f = xef ^ xgh ^ h;

		//	(c b a) + (g f e) → (c b a)		論理演算回数：９
		d = a & e;
		a ^= e;
		h = b & f;
		b ^= f;
		h |= b & d;			//	d は b0 からの桁上り
		b ^= d;
		c ^= g ^ h;

		//x ~ s3まで 論理演算回数：５
		auto x = ~c & b;

		// a~hの各ビットごとの合計値を計算
		// 例 : s2には、合計値が2の時のみそのビットがOnになる
		auto s2 = x & ~a;
		auto s3 = x & a;

		// ライフゲームの生存条件に当てはめて返す
		return (data & s2) | s3;
	}
}