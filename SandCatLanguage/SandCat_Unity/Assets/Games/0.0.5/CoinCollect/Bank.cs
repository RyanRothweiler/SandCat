using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bank : MonoBehaviour
{

	public int arrayLength;

	public GameObject coinFab;

	// Use this for initialization
	void Start ()
	{
		for (int index = 1; index < arrayLength + 1; index++) {
			GameObject obj = Instantiate(coinFab);
			obj.SetActive(true);
			obj.GetComponent<Coin>().index = index;
			obj.transform.parent = this.transform.parent;
		}
	}

	// Update is called once per frame
	void Update ()
	{

	}
}
