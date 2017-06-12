using System.Collections;
using System.Collections.Generic;
using UnityEngine;


// Sets obj to active if fluentName is above the value.
public class DisplayIfFluent : MonoBehaviour
{

	public string fluentName;
	public int rightVal;
	public GameObject obj;
	public Comparison comparison;

	// Update is called once per frame
	void Update ()
	{
		int leftVal = (int)SandCat.instance.GetFluentValue(fluentName);
		if (comparison.IsTrue(leftVal, rightVal)) {
			obj.SetActive(true);
		} else {
			obj.SetActive(false);
		}
	}
}
