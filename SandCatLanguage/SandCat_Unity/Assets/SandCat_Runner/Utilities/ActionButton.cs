using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

// This class binds an action to a button. Button is dark if the action is not valid.

[RequireComponent(typeof(Button))]
public class ActionButton : MonoBehaviour
{

	public string actionName;
	private Button button;

	// Use this for initialization
	void Start ()
	{
		button = this.GetComponent<Button>();
		button.onClick.AddListener(OnClick);
	}

	public void OnClick()
	{
		SandCat.instance.DoAction(actionName);
	}
}
