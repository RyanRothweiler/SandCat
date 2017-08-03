using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SSWorker : MonoBehaviour
{
	public Grid grid;
	public int workerIndex;

	void Update ()
	{
		// place worker on board
		{
			int xPos = (int)SandCat.instance.GetFluentInArray("Workers", workerIndex, "X");
			int yPos = (int)SandCat.instance.GetFluentInArray("Workers", workerIndex, "Y");
			this.transform.position = grid.GridToWorldPos(new Vector2(xPos, yPos));
		}


		// worker exhausted display
		{
			// bool isExhaused = false;
			// int exVal = (int)SandCat.instance.GetFluentInArray("Workers", workerIndex, "Exhausted");
			// if (exVal == 0) {
			// 	isExhaused = false;
			// } else {
			// 	isExhaused = true;
			// }
		}
	}
}
