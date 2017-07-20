using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Car : MonoBehaviour
{

	public Grid board;

	public Vector3[] prevPositions = new Vector3[5];

	private LineRenderer liner;

	// Use this for initialization
	void Start ()
	{
		liner = this.GetComponent<LineRenderer>();
	}

	// Update is called once per frame
	void Update ()
	{
		int x = (int)SandCat.instance.GetFluentValue("PlayerX");
		int y = (int)SandCat.instance.GetFluentValue("PlayerY");
		this.transform.position = board.GridToWorldPos(new Vector2(x, y));

		for (int index = 0; index < prevPositions.Length; index++) {
			liner.SetPosition(index, prevPositions[index]);
		}
	}

	public void MoveCar(string actionName)
	{
		for (int index = 0; index < prevPositions.Length - 1; index++) {
			prevPositions[index] = prevPositions[index + 1];
		}

		SandCat.instance.DoAction(actionName);

		int x = (int)SandCat.instance.GetFluentValue("PlayerX");
		int y = (int)SandCat.instance.GetFluentValue("PlayerY");
		this.transform.position = board.GridToWorldPos(new Vector2(x, y));

		prevPositions[prevPositions.Length - 1] = this.transform.position;
	}
}
