using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Sets obj to active if fluentName is above the value.
public class DisplayIfFluent : MonoBehaviour
{

	public string fluentName;
	public float val;
	public GameObject obj;

	// Update is called once per frame
	void Update ()
	{
		if (SandCat.instance.GetFluentValue(fluentName) > val) {
			obj.SetActive(true);
		} else {
			obj.SetActive(false);
		}
	}
}
