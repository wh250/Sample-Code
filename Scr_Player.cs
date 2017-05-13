using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using UnityEngine;

//Unity, c#script for player behavior
//Uses razer hydra plugin
public class Scr_Player : MonoBehaviour {
    //razer hydra info
    RazerHydraPlugin plugin = new RazerHydraPlugin();
    int left = 0;
    int right = 1;
    //sound files
    public AudioClip exp;
    public AudioClip bul;
    //audiosource
    AudioSource au;
    //gameobject for bullet
    public GameObject bullet;
    //velocity and acceleration vectors
    public Vector3 vel;
    public Vector3 acc;
    //which planet gravity is oriented towards
    public int ori;
    //rotation of orientation
    Quaternion orientation;
    //array of planets which exert gravity
    public GameObject[] planets;
    //health
    public int health=100;
    //is player on ground
    public bool ground;
    //healthbar
    public RectTransform healthbar;
    //own physics body
    Rigidbody self;
    //score
    public int score=0;
    //temporary string
    string s;
    //temporary value for reload
    int x=0;

    //damage function
    public void damage(int a)
    {
        //if positive damage play damaged sound
        if (a > 0)
        {
            au.PlayOneShot(exp, 1);
        }
        //lose life
        health -= a;
        if (health > 100)
        {
            health = 100;
        }
        //adjust healthbar
        healthbar.sizeDelta = new Vector2(health, healthbar.sizeDelta.y);
        //if you die go to menu scene
        if (health <= 0)
        {
            SceneManager.LoadScene("menu");
        }
    }

	// Use this for initialization
	void Start () {
        //initialize razer hydra, physics body, velocity, orientation, and audio source
        plugin.init();
        self = gameObject.GetComponent<Rigidbody>();
        vel = Vector3.zero;
        ori = 0;
        au = GetComponent<AudioSource>();
    }

	// Update is called once per frame
	void Update () {
        //if x>0 decrement it until it reaches 0
        if (x > 0) { x--; }
        //quit hotkey
        if (Input.GetKeyDown("q"))
        {
            SceneManager.LoadScene("menu");
        }
        //score string
        s = "Score: ";
        s += score;
        // update score display
        transform.GetChild(0).GetChild(0).GetChild(1).GetComponent<Text>().text = s;
        //check if touching ground
        ground = Physics.Raycast(transform.position, -transform.up, .5F, 1 << 8);
        //if touching ground, make child of planet it is touching
        if (ground)
        {
            transform.parent = planets[ori].transform;
        }
        //otherwise no parent
        else
        {
            transform.parent = null;
        }
        //reset acceleration vector
        acc = Vector3.zero;
        //get razerhydra input (left controller)
        plugin.getNewestData(left);
        //rotate camera according to controller
        transform.GetChild(0).localEulerAngles = new Vector3(plugin.data.position.y / -8, plugin.data.position.x / 8, 0);
        //if touching ground, rotate based on joystick
        if (ground)
        {
            transform.Translate(Vector3.forward * plugin.data.joystick_y * .2F, Space.Self);
            transform.Rotate(Vector3.up * plugin.data.joystick_x, Space.Self);
        }
        //otherwise tilt based on joystick
        else
        {
            transform.Rotate(Vector3.right * plugin.data.joystick_y * 2, Space.Self);
            transform.Rotate(Vector3.back * plugin.data.joystick_x * 2, Space.Self);
        }
        //if trigger is pressed jump up
        if (plugin.data.trigger > 0.8F)
        {
            acc += transform.up * 20;
        }
        //get data from right controller
        plugin.getNewestData(right);
        //aim gun with right controller
        transform.GetChild(1).localEulerAngles = new Vector3(plugin.data.position.y / -8, plugin.data.position.x / 8, 0);
        //if trigger is pressed
        if (plugin.data.trigger > 0.8F)
        {
            //play fire sound
            au.PlayOneShot(bul, 1);
            //make bullet, point bullet in direction of gun
            Instantiate(bullet, transform.GetChild(1).GetChild(0).GetChild(1).position, Quaternion.identity, transform);
            transform.GetChild(transform.childCount - 1).gameObject.GetComponent<Scr_Bullet>().planets = planets;
            transform.GetChild(transform.childCount - 1).gameObject.GetComponent<Scr_Bullet>().vel = Vector3.Normalize(transform.GetChild(1).GetChild(0).GetChild(1).position - transform.GetChild(1).GetChild(0).GetChild(0).position) * 10;
        }
        //if not on ground
        if (!ground)
        {
            //add all planets gravity onto player
            for (int i = 0; i < planets.Length; i++)
            {
                //F=constant which is 1*mass/R^2
                //in this case planet position-position/R^3
                acc += planets[i].GetComponent<Rigidbody>().mass * (planets[i].transform.position - transform.position) / Mathf.Pow(Vector3.Distance(transform.position, planets[i].transform.position), 3);
            }
        }
        for (int i = 0; i < planets.Length; i++)
        {
            //player orientation is toward planet with strongest gravity
            if(Mathf.Pow(Vector3.Distance(transform.position, planets[ori].transform.position), 2)/ planets[ori].GetComponent<Rigidbody>().mass > Mathf.Pow(Vector3.Distance(transform.position, planets[i].transform.position), 2)/ planets[i].GetComponent<Rigidbody>().mass)
            {
                ori = i;
            }
        }

        //find orientation based on forward and planet gravity vector
        orientation = Quaternion.LookRotation(Vector3.Cross(transform.position - planets[ori].transform.position,Vector3.Cross(transform.forward, transform.position - planets[ori].transform.position)),transform.position - planets[ori].transform.position);
        //if not on ground rotate player towards orientatation
        //player should be upright from planet which gravity is strongest
        if (!ground || Quaternion.Angle(orientation,transform.rotation)>.05F)
        {
            transform.rotation = Quaternion.RotateTowards(transform.rotation, orientation, Quaternion.Angle(orientation,transform.rotation)*.1F);
        }
        //keyboard input incase no razer hydra
        //jump
        if (Input.GetKeyDown("space"))
        {
            acc += (transform.up+transform.forward) * 40;
        }
        //fire
        if (Input.GetKey("p") && x==0)
        {
            x = 3;
            au.PlayOneShot(bul, 1);
            Instantiate(bullet, transform.GetChild(1).GetChild(0).GetChild(1).position, Quaternion.identity, transform);
            transform.GetChild(transform.childCount - 1).gameObject.GetComponent<Scr_Bullet>().planets = planets;
            transform.GetChild(transform.childCount - 1).gameObject.GetComponent<Scr_Bullet>().vel = Vector3.Normalize(transform.GetChild(1).GetChild(0).GetChild(1).position - transform.GetChild(1).GetChild(0).GetChild(0).position) * 10+self.velocity;
        }
        //movement
        if (ground)
        {
            if (Input.GetKey("w"))
            {
                transform.Translate(Vector3.forward * .05F, Space.Self);
            }
            if (Input.GetKey("s"))
            {
                transform.Translate(Vector3.back * .05F, Space.Self);
            }
            if (Input.GetKey("a"))
            {
                transform.Rotate(Vector3.down, Space.Self);
            }
            if (Input.GetKey("d"))
            {
                transform.Rotate(Vector3.up, Space.Self);
            }
        }
        else
        {
            if (Input.GetKey("w"))
            {
                transform.Rotate(Vector3.right*2, Space.Self);
            }
            if (Input.GetKey("s"))
            {
                transform.Rotate(Vector3.left*2, Space.Self);
            }
            if (Input.GetKey("a"))
            {
                transform.Rotate(Vector3.forward*2, Space.Self);
            }
            if (Input.GetKey("d"))
            {
                transform.Rotate(Vector3.back*2, Space.Self);
            }
        }
        //add acceleration to velocity
        self.velocity += acc;
        //drag
        self.velocity = self.velocity * .95F;
	}
}