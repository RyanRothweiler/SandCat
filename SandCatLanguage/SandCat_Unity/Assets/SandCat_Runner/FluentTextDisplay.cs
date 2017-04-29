using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Text))]
public class FluentTextDisplay : MonoBehaviour
{
	public string fluentName;
	private Text display;

	// Use this for initialization
	void Start ()
	{
		if (SandCat.instance == null) {
			Debug.LogError("FluentTextDisplay requires a cat to be present.");
		}
		display = this.GetComponent<Text>();
	}

	// Update is called once per frame
	void Update ()
	{
		display.text = SandCat.instance.GetFluentValue(fluentName).ToString();
	}
}
