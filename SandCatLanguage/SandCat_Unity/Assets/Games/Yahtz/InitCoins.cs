using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InitCoins : MonoBehaviour
{
	public Grid board;
	public int arrayIndex;

	// Use this for initialization
	void Start ()
	{

	}

	// Update is called once per frame
	void Update ()
	{
		string arrayName = "InitCoins";
		if ((int)SandCat.instance.GetFluentInArray(arrayName, arrayIndex, "Alive") == 1) {
			int x = (int)SandCat.instance.GetFluentInArray(arrayName, arrayIndex, "X");
			int y = (int)SandCat.instance.GetFluentInArray(arrayName, arrayIndex, "Y");
			this.transform.position = board.GridToWorldPos(new Vector2(x, y));
		} else {
			Destroy(this.gameObject);
		}
	}
}
