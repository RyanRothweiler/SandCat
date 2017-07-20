using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Worker : MonoBehaviour
{

	public string arrayName;
	public int arrayIndex;

	public Grid board;

	private Vector3 origScale;

	void Start()
	{
		origScale = this.transform.localScale;
	}

	void Update ()
	{
		// place worker on board
		{
			int x = (int)SandCat.instance.GetFluentInArray(arrayName, arrayIndex, "X");
			int y = (int)SandCat.instance.GetFluentInArray(arrayName, arrayIndex, "Y");
			this.transform.position = board.GridToWorldPos(new Vector2(x, y));
		}

		// Highlight if selected
		{
			if ((int)SandCat.instance.GetFluentValue("WorkerIndexMoving") == arrayIndex) {
				this.transform.localScale = origScale * 1.5f;
			} else {
				this.transform.localScale = origScale;
			}
		}
	}
}
