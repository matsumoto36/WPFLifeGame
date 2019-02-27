using System.Windows;
using LifeGameSystem;

namespace LifeGameSimulator {

	/// <summary>
	/// マウス操作のツール用インターフェース
	/// </summary>
	interface ITool {

		void OnMouseDown(LifeGameModel lifeGameModel, Point mousePosition);
		void OnMouseMove(LifeGameModel lifeGameModel, Point mousePosition);
		void OnMouseUp(LifeGameModel lifeGameModel, Point mousePosition);

	}
}
