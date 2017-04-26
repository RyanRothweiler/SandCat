using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class SandCat_UnityRunner : MonoBehaviour
{

	[DllImport ("SandCat_Runner")]
	private static extern int LoadProg(string progName);

	// Use this for initialization
	void Start ()
	{
		Debug.Log(LoadProg("hey what"));
	}

	// Update is called once per frame
	void Update ()
	{

	}
}
